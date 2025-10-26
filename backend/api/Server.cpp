#include "api/Server.hpp"
#include "nlohmann/json.hpp"
#include "sim/battle/Battle.hpp"
#include "sim/data/Abilities.hpp"
#include "sim/data/Moves.hpp"
#include <chrono>
#include <thread>
#include "./Utils.hpp"
#include "sim/tournament/Tournament.hpp"
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include "tflib/strings.h"


using namespace tfhttp;
using json = nlohmann::json;

#define ALLOWED_HEADERS "Authorization,Content-Type,X-Requested-With,Set-Cookie"
#define ALLOWED_METHODS "GET,POST,PUT,DELETE,OPTIONS"

constexpr int MAX_ALLOWED_LOGIN_ATTEMPTS = 5;


void preflightHandler(const Request& req, Response& res){
    res.headers["Access-Control-Allow-Methods"] = ALLOWED_METHODS;
    res.headers["Access-Control-Allow-Headers"] = ALLOWED_HEADERS;
    res.Send("");
}

bool debugDelayMiddleware(const Request& req, Response& res){
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return true;
}

bool jsonContentTypeMiddleware(const Request& req, Response& res){
    res.headers["Content-Type"] = "application/json";
    return true;
}

std::string createAllDataResponse(){
    json response;

    //Inefficient, but only runs once on startup so not a big deal
    json speciesResponse = json::parse(createSpeciesDataResponse());
    json abilityResponse = json::parse(createAbilityDataResponse());
    json itemResponse = json::parse(createItemDataResponse());
    json natureResponse = json::parse(createNatureDataResponse());
    json moveResponse = json::parse(createMoveDataResponse());
    json trainerLevelResponse = json::parse(createTrainerLevelDataResponse());

    response["success"] = true;
    response["message"] = "OK";
    response["data"] = {
        {"speciesList", speciesResponse["data"].get<std::vector<std::string>>()},
        {"abilityList", abilityResponse["data"].get<std::vector<std::string>>()},
        {"itemList", itemResponse["data"].get<std::vector<std::string>>()},
        {"natureList", natureResponse["data"].get<std::vector<std::string>>()},
        {"moveList", moveResponse["data"].get<std::vector<std::string>>()},
        {"trainerLevelList", trainerLevelResponse["data"].get<std::vector<std::string>>()}
    };
    return response.dump();
}

size_t npcs::Server::createTournamentRequest(const json& json, size_t user, const std::string& name, const std::string& ip){
    size_t id = db.createEmptyTournament(user, name, ip);
    TournamentRequest request{
        .requestJson = json,
        .id = id,
        .user = user
    };
    threadCounterMutex.lock();
    int threadNumber = tournamentRequestThreadCounter++;
    tournamentRequestThreadCounter = tournamentRequestThreadCounter % max_tournament_threads;
    threadCounterMutex.unlock();
    auto& queue = queuedTournaments[threadNumber];
    auto& mutex = queuedTournamentMutexes[threadNumber];
    mutex.lock();
    idToThreadMutex.lock();
    idToThread[id] = threadNumber;
    idToThreadMutex.unlock();
    std::cout << "Queing up a tournament on thread #" << threadNumber << '\n';
    queue.push_back(request);
    mutex.unlock();

    return id;
}

static const std::string TOKEN_COOKIE_START = "token=";
static const std::string BEARER_AUTH_SCHEME = "Bearer ";

static Credentials getCredentialsFromRequest(const Request& req){
    Credentials result;
    if (req.Has_Header("Cookie")){
        result.type = ClientType::BROWSER;
        std::stringstream cookieHeader(req.Get_Header("Cookie"));
        for (std::string cookie; std::getline(cookieHeader, cookie, ';');){
            cookie = tflib::trim(cookie);
            if (cookie.find(TOKEN_COOKIE_START) != 0){
                continue;
            }
            result.token = cookie.substr(TOKEN_COOKIE_START.size());
            return result;
        }
        result.error = "'token' cookie not supplied.";
        return result;
    }
    else if (req.Has_Header("Authorization")){
        result.type = ClientType::SCRIPT;
        const std::string& authHeader = req.Get_Header("Authorization");
        size_t bearerPos = authHeader.find(BEARER_AUTH_SCHEME);
        if (bearerPos != 0){
            result.error = "Invalid authentication scheme. Only Bearer is supported. (ie. 'Bearer <token>')";
            return result;
        }
        result.token = authHeader.substr(BEARER_AUTH_SCHEME.size());
        return result;
    }
    else{
        result.type = ClientType::UNKNOWN;
        result.error = "No 'Cookie' or 'Authorization' header provided.";
        return result;
    }
}

inline void sendProblemResponse(std::string& problems, json& response, Response& res){
    if (problems[problems.size() - 1] == '\n'){
        problems.pop_back();
    }
    response["message"] = problems;
    response["success"] = false;
    res.Set_Status(400);
    res.Send(response.dump());
}

npcs::Server::Server() :
 SPECIES_DATA_RESPONSE{createSpeciesDataResponse()},
 ABILITY_DATA_RESPONSE{createAbilityDataResponse()},
 ITEM_DATA_RESPONSE{createItemDataResponse()},
 NATURE_DATA_RESPONSE{createNatureDataResponse()},
 MOVE_DATA_RESPONSE{createMoveDataResponse()},
 ALL_DATA_RESPONSE{createAllDataResponse()}
 {

    if (max_tournament_threads < 1){
        throw std::runtime_error("Invalid max tournament threads in ini file: " + config.get("tournament_threads"));
    }

    if (max_trainers_per_user < 1){
        throw std::runtime_error("Invalid max trainers per user in ini file: " + config.get("max_trainers_per_user"));
    }

    queuedTournaments = new std::deque<TournamentRequest>[max_tournament_threads];
    queuedTournamentMutexes = new std::mutex[max_tournament_threads];

    std::cout << "Deleting unsaved tournaments older than " << keepTournamentDays << " days...\n";
    int numDeletedTournaments = db.deleteOldTournaments(keepTournamentDays);
    std::cout << "Deleted " << numDeletedTournaments << " tournaments.\n";
    

    //Create routes
    Route* baseRoute = app.Create_Route("/api");
    Route* authorizedRoute = app.Create_Route("/api/user/:username");

    auto addCORSHeaderMiddleware = [=, this](const Request& req, Response& res){
        res.headers["Access-Control-Allow-Origin"] = websiteURL;
        res.headers["Access-Control-Allow-Credentials"] = "true";
        return true;
    };

    //Add middlewares

    // CORS
    app.base_route.Use(addCORSHeaderMiddleware);
    baseRoute->Use(addCORSHeaderMiddleware);
    authorizedRoute->Use(addCORSHeaderMiddleware);

    //Authorization
    authorizedRoute->Use([=, this](const Request& req, Response& res){
        json response;
        response["success"] = false;
        std::string username = req.path_params.at("username");

        auto creds = getCredentialsFromRequest(req);
        if (creds.error != ""){
            response["message"] = creds.error;
            res.Set_Status(401);
            res.Send(response.dump());
            return false;
        }

        if (db.isTokenValid(username, creds.token)){
            db.updateTokenLastUsed(username, creds.token);
            if (creds.type == ClientType::BROWSER) res.headers["Set-Cookie"] = "token=" + creds.token + "; Max-Age=2147483647; HttpOnly; Secure; Path=/; SameSite=Strict; Domain=" + domain;
            return true;
        }
        else{
            response["message"] = "Unauthorized: Invalid token.";
            res.Set_Status(401);
            res.Send(response.dump());
            return false;
        }

    });

    // Content type
    baseRoute->Use(jsonContentTypeMiddleware);
    authorizedRoute->Use(jsonContentTypeMiddleware);

    // Fake delay for debugging
#if DEBUG_DELAY
    baseRoute->Use(debugDelayMiddleware);
    authorizedRoute->Use(debugDelayMiddleware);
#endif

    // If serving frontend
    if (serveStatic){
        app.Set_Static_Directory(staticDir);
        app.Set_Default_Handler([=,this] (const Request& req, Response& res){
            res.Send_File(staticDir + "/index.html");
        });
        std::cout << "Serving static files in " << staticDir << '\n';
    }

    //Add preflight handler to all paths
    app.Add_Handler("OPTIONS", "*", preflightHandler);

    //Add API handlers 
    app.Add_Handler("POST", baseRoute, "/auth", [=, this](const Request& req, Response& res){
        json response;
        {
            std::unique_lock lk(ipFailedLoginsMutex);
            if (!ipFailedLogins[req.ip].canTryAgain()){
                response["message"] = "Too many failed logins from your IP. Please wait before trying again.";
                response["success"] = false;
                res.Set_Status(429);
                res.Send(response.dump());
                return;
            }
        }

        json body;
        try {
            body = json::parse(req.body);
        }
        catch (const json::parse_error& e){
            response["success"] = false;
            response["message"] = "Bad Request: " + std::string(e.what());
            res.Set_Status(400);
            res.Send(response.dump());
            return;
        }
        std::string problems = validateAuthRequestSchema(body);
        if (problems != ""){
            sendProblemResponse(problems, response, res);
            return;
        }
        std::string token;
        problems = db.createUserSession(body["username"], body["password"], token);

        if (problems != ""){
            if (problems == "Invalid credentials"){
                std::unique_lock lk(ipFailedLoginsMutex);
                ipFailedLogins[req.ip].increment();
            }
            response["message"] = problems;
            response["success"] = false;
            res.Set_Status(401);
            res.Send(response.dump());
            return;
        }

        res.headers["Set-Cookie"] = "token=" + token + "; Max-Age=2147483647; HttpOnly; Secure; Path=/; SameSite=Strict; Domain=" + domain;

        response["success"] = true;
        response["message"] = "OK";
        response["token"] = token;
        res.Send(response.dump());
    });

    app.Add_Handler("GET", authorizedRoute, "/", [=, this](const Request& req, Response& res){
        json response;

        auto dbUser = db.getUserData(req.path_params.at("username"));

        if (!dbUser.has_value()){
            response["success"] = false;
            response["message"] = "That user doesn't exist.";
            res.Send(response.dump());
            return;
        }

        auto& user = dbUser.value();

        json data;
        data["id"] = std::to_string(user.id);
        data["name"] = user.name;
        data["accountCreated"] = user.accountCreated;
        data["lastPasswordChange"] = user.lastPasswordChange;
        data["isAdmin"] = user.isAdmin;
        response["success"] = true;
        response["message"] = "OK";
        response["data"] = data;
        res.Send(response.dump());
    });

    app.Add_Handler("DELETE", authorizedRoute, "/logout", [=, this](const Request& req, Response& res){
        json response;
        response["success"] = false;
        auto creds = getCredentialsFromRequest(req);
        if (creds.error != ""){
            response["message"] = creds.error;
            res.Set_Status(401);
            res.Send(response.dump());
            return;
        }
        bool success = db.deleteUserSession(req.path_params.at("username"), creds.token);
        if (!success){
            response["message"] = "Unauthorized: Invalid token.";
            res.Set_Status(401);
            res.Send(response.dump());
            return;
        }
        response["success"] = true;
        response["message"] = "OK";
        res.Send(response.dump());
    });

    app.Add_Handler("PUT", authorizedRoute, "/password", [=, this](const Request& req, Response& res){
        json response;
        response["success"] = false;
        json request;
        try {
            request = json::parse(req.body);
        }
        catch (const json::parse_error& e){
            response["message"] = "Bad Request: " + std::string(e.what());
            res.Set_Status(400);
            res.Send(response.dump());
            return;
        }

        std::string problems = validateUpdatePasswordRequest(request);

        if (problems != ""){
            sendProblemResponse(problems, response, res);
            return;
        }

        std::string currentPassword = request["currentPassword"].get<std::string>();
        std::string newPassword = request["newPassword"].get<std::string>();

        if (currentPassword == newPassword){
            response["message"] = "New password is the same as the old password.";
            res.Set_Status(400);
            res.Send(response.dump());
            return;
        }

        if (!db.isUserPasswordCorrect(req.path_params.at("username"), currentPassword)){
            response["message"] = "Incorrect password.";
            res.Set_Status(400);
            res.Send(response.dump());
            return;
        }

        db.updateUserPassword(req.path_params.at("username"), newPassword);

        response["success"] = true;
        response["message"] = "OK";
        res.Send(response.dump());
    });

    app.Add_Handler("GET", baseRoute, "/data/species", [=,this](const Request& req, Response& res){
        res.Send(SPECIES_DATA_RESPONSE);
    });

    app.Add_Handler("GET", baseRoute, "/data", [=,this](const Request& req, Response& res){
        res.Send(ALL_DATA_RESPONSE);
    });

    app.Add_Handler("POST", baseRoute, "/battle", [=, this](const Request& req, Response& res){
        json response;
        response["success"] = false;
        json request;
        try {
            request = json::parse(req.body);
        }
        catch (const json::parse_error& e){
            response["message"] = "Bad Request: " + std::string(e.what());
            res.Set_Status(400);
            res.Send(response.dump());
            return;
        }
        std::string problems = validateBattleRequest(request);
        if (problems != ""){
            sendProblemResponse(problems, response, res);
            return;
        }

        // Simulate Battle
        Trainer trainer1(request["trainer1"]);
        Trainer trainer2(request["trainer2"]);
        size_t seed = seedFromString(request["seed"].get<std::string>());
        Battle battle(trainer1, trainer2, seed);
        battle.simulate();

        if (battle.invalid){
            db.saveErrorBattle(request.dump());
            response["message"] = "Sorry, something went wrong with that battle. A bug report has been submitted.";
            res.Set_Status(500);
            res.Send(response.dump());
            return;
        }

        std::string type = request["type"].get<std::string>();
        json data = type == "events" ? battle.eventsJson() : json(battle.textLog());

        response["success"] = true;
        response["data"] = data;
        response["message"] = "OK";
        res.Send(response.dump());
    });
    
    app.Add_Handler("POST", baseRoute, "/tournament", [=,this](const Request& req, Response& res){
        json response;
        response["success"] = false;
        response["id"] = "-1";
        json request;
        try {
            request = json::parse(req.body);
        }
        catch (const json::parse_error& e){
            response["message"] = "Bad Request: " + std::string(e.what());
            res.Set_Status(400);
            res.Send(response.dump());
            return;
        }
        std::string problems = validateTournamentRequest(request);
        if (problems != ""){
            sendProblemResponse(problems, response, res);
            return;
        }
        size_t userID = 0;
        if (request.contains("user")){
            std::string username = request["user"].get<std::string>();
            auto creds = getCredentialsFromRequest(req);
            if (creds.error != ""){
                response["message"] = creds.error;
                res.Set_Status(401);
                res.Send(response.dump());
                return;
            }
            if (db.isTokenValid(username, creds.token)){
                db.updateTokenLastUsed(username, creds.token);
                userID = db.userIdFromName(username);
            }
            else{
                response["message"] = "Unauthorized: Invalid token.";
                res.Set_Status(401);
                res.Send(response.dump());
                return;
            }
        }
        size_t ipTournaments = db.getTournamentsFromIPToday(req.ip);
        std::cout << ipTournaments << '\n';
        if (ipTournaments >= maxTournamentsPerDay){
            response["message"] = "Too many tournaments requested from your IP today. Please try again tomorrow.";
            res.Set_Status(429);
            res.Send(response.dump());
            return;
        }

        std::string tournamentName = "";
        if (request.contains("name")){
            tournamentName = tflib::trim(request["name"].get<std::string>());
        }

        size_t id  = createTournamentRequest(request, userID, tournamentName, req.ip);

        // Send back tournament ID
        response["success"] = true;
        response["id"] = std::to_string(id);
        response["message"] = "OK";
        res.Send(response.dump());
    });

    app.Add_Handler("GET", baseRoute, "/trainer/:id", [=, this](const Request& req, Response& res){
        json response;
        response["success"] = false;
        size_t trainerID = 0;
        try{
            trainerID = stoul(req.path_params.at("id"));
        }
        catch (...){
            response["message"] = "Sorry, that trainer doesn't exist.";
            res.Set_Status(404);
            res.Send(response.dump());
            return;
        }

        if (!db.tournamentTrainerExists(trainerID)){
            response["message"] = "Sorry, that trainer doesn't exist.";
            res.Set_Status(404);
            res.Send(response.dump());
            return;
        }

        auto trainerQueryResult = db.getTrainer(trainerID);

        response["message"] = "OK";
        response["success"] = true;
        response["data"] = trainerQueryResult.value().toJSON();
        res.Send(response.dump());
    });

    app.Add_Handler("GET", baseRoute, "/tournament/:id", [=,this](const Request& req, Response& res){
        json response;
        response["success"] = false;
        size_t tournamentID = 0;
        try{
            tournamentID = stoul(req.path_params.at("id"));
        }
        catch (...){
            response["message"] = "Sorry, that tournament doesn't exist.";
            res.Set_Status(404);
            res.Send(response.dump());
            return;
        }
        int threadNumber;
        try{
            std::unique_lock lk(idToThreadMutex);
            threadNumber = idToThread.at(tournamentID);
        }
        catch (const std::out_of_range& e){
            threadNumber = -1;
        }
        if (threadNumber >= 0){
            // Tournament is being run currently, look it up in the queue
            std::unique_lock lk(queuedTournamentMutexes[threadNumber]);
            auto& queue = queuedTournaments[threadNumber];
            int position = -1;
            for (int i = 0; i < queue.size(); i++){
                const auto& request = queue[i];
                if (request.id == tournamentID){
                        position = i;
                        break;
                }
            }

            if (position >= 0){
                std::string message = position == 0 ? "Please wait. Your tournament is currently being simulated." : "Please wait. Your tournament is in queue at position " + std::to_string(position) + ".";
                response["message"] = message;
                res.Set_Status(202);
                response["success"] = true;
                response["data"] = {{"status", "queued"}};
                res.Send(response.dump());
                return;
            }
            else{
                /*
                If this block is ever reached, it should mean that the tournament just got removed from the queue
                and we can assume that it has been saved to the DB, and it should be ready.
                */
                auto trQueryResult = db.getTournament(tournamentID);
                if (trQueryResult.has_value()){
                    const TournamentResults& tr = trQueryResult.value();
                    if (tr.ready){
                        response["success"] = true;
                        response["data"] = tr.toJSON();
                        response["data"]["status"] = "done";
                        res.Send(response.dump());
                        return;
                    }
                    else{
                        /*
                        If the app is working properly, this block should never be reached. This should only ever
                        be reached if a tournament just errored.
                        */
                    #ifdef NDEBUG
                        response["message"] = "Sorry, that tournament doesn't exist.";
                        res.Set_Status(404);
                        response["success"] = false;
                        res.Send(response.dump());
                        return;
                    #else
                        throw std::runtime_error("Tournament wasn't ready: " + std::to_string(tournamentID));
                    #endif
                    }
                }
                else{
                    //This block should never run. If a tournament was just in queue, it should have a record in the DB.
                #ifdef NDEBUG
                    response["message"] = "Sorry, that tournament doesn't exist.";
                    res.Set_Status(404);
                    response["success"] = false;
                    res.Send(response.dump());
                    return;
                #else
                    throw std::runtime_error("Tournament was just in queue but somehow not in database: " + std::to_string(tournamentID));
                #endif
                }
            }
        }
        else{
            // Tournament is not being run currently, look it up in the DB
            auto trQueryResult = db.getTournament(tournamentID);
            if (trQueryResult.has_value()){
                const TournamentResults& tr = trQueryResult.value();
                if (tr.ready){
                    response["success"] = true;
                    response["data"] = tr.toJSON();
                    response["data"]["status"] = "done";
                    res.Send(response.dump());
                    return;
                }
                else{
                    // If the tournament isn't being run and its not ready, something went wrong with it. 
                    response["message"] = "Sorry, something went wrong with this tournament. Please try sending it again with a different seed.";
                    res.Set_Status(404);
                    response["success"] = false;
                    res.Send(response.dump());
                    return;
                }
            }
            else{
                // If its not being run and its not in the DB, it doesn't exist
                response["message"] = "Sorry, that tournament doesn't exist.";
                res.Set_Status(404);
                response["success"] = false;
                res.Send(response.dump());
                return;
            }
        }
    });

    app.Add_Handler("GET", authorizedRoute, "/trainers", [=, this](const Request& req, Response& res){
        std::string username = req.path_params.at("username");

        std::vector<json> userTrainerJSONs = db.getUserTrainers(username);

        json response;
        response["success"] = true;
        response["message"] = "OK";
        response["data"] = userTrainerJSONs;

        res.Send(response.dump());
    });

    app.Add_Handler("POST", authorizedRoute, "/trainer", [=, this](const Request& req, Response& res){
        json response;
        response["success"] = false;
        response["id"] = "-1";
        json request;
        try {
            request = json::parse(req.body);
        }
        catch (const json::parse_error& e){
            response["message"] = "Bad Request: " + std::string(e.what());
            res.Set_Status(400);
            res.Send(response.dump());
            return;
        }
        std::string problems = validateTrainerJSON(request, "");
        if (problems != ""){
            sendProblemResponse(problems, response, res);
            return;
        }

        size_t userID = db.userIdFromName(req.path_params.at("username"));

        if (db.userTrainerCount(userID) >= max_trainers_per_user){
            response["message"] = "Your user already has the maximum number of allowed trainers (" + std::to_string(max_trainers_per_user) + "). Please delete some to make room.";
            response["success"] = false;
            response["id"] = -1;
            res.Set_Status(409);
            res.Send(response.dump());
            return;
        }

        size_t trainerID = db.saveTrainer(request, userID, 0);
        
        response["message"] = "OK";
        response["success"] = true;
        response["id"] = std::to_string(trainerID);
        res.Send(response.dump());
        return;

    });

    app.Add_Handler("PUT", authorizedRoute, "/trainer/:id", [=, this](const Request& req, Response& res){
        json response;
        response["success"] = false;
        size_t trainerID = 0;
        try{
            trainerID = stoul(req.path_params.at("id"));
        }
        catch (...){
            response["message"] = "Sorry, that trainer doesn't exist.";
            res.Set_Status(404);
            res.Send(response.dump());
            return;
        }
        json request;
        try {
            request = json::parse(req.body);
        }
        catch (const json::parse_error& e){
            response["message"] = "Bad Request: " + std::string(e.what());
            res.Set_Status(400);
            res.Send(response.dump());
            return;
        }
        std::string problems = validateTrainerJSON(request, "");
        if (problems != ""){
            sendProblemResponse(problems, response, res);
            return;
        }

        size_t userID = db.userIdFromName(req.path_params.at("username"));

        if (db.userTrainerExists(trainerID, userID)){
            db.deleteSavedTrainer(userID, trainerID);
            db.saveTrainer(request, userID, 0, trainerID);
            response["success"] = true;
            response["message"] = "OK";
            res.Send(response.dump());
            return;
        }
        else{
            response["message"] = "Sorry, that trainer doesn't exist.";
            res.Set_Status(404);
            res.Send(response.dump());
            return;
        }
    });

    app.Add_Handler("DELETE", authorizedRoute, "/trainer/:id", [=, this](const Request& req, Response& res){
        json response;
        response["success"] = false;
        size_t trainerID = 0;
        try{
            trainerID = stoul(req.path_params.at("id"));
        }
        catch (...){
            response["message"] = "Sorry, that trainer doesn't exist.";
            res.Set_Status(404);
            res.Send(response.dump());
            return;
        }
        size_t userID = db.userIdFromName(req.path_params.at("username"));
        if (db.userTrainerExists(trainerID, userID)){
            db.deleteSavedTrainer(userID, trainerID);
            response["success"] = true;
            response["message"] = "OK";
            res.Send(response.dump());
            return;
        }
        else{
            response["message"] = "Sorry, that trainer doesn't exist.";
            res.Set_Status(404);
            res.Send(response.dump());
            return;
        }
    });

    app.Add_Handler("GET", authorizedRoute, "/tournaments", [=, this](const Request& req, Response& res){
        std::vector<TournamentResults> savedTournaments = db.getUserTournaments(req.path_params.at("username"));
        std::vector<json> savedTournamentJSONs;
        savedTournamentJSONs.reserve(savedTournaments.size());

        for (auto& tournament : savedTournaments){
            savedTournamentJSONs.push_back(tournament.toJSON());
        }

        json response;
        response["success"] = true;
        response["message"] = "OK";
        response["data"] = savedTournamentJSONs;

        res.Send(response.dump());
    });

    app.Add_Handler("POST", authorizedRoute, "/tournament/:id", [=, this](const Request& req, Response& res){
        json response;
        response["success"] = false;
        response["id"] = "-1";

        size_t tournamentID = 0;
        try{
            tournamentID = stoul(req.path_params.at("id"));
        }
        catch (...){
            response["message"] = "Sorry, that tournament doesn't exist.";
            res.Set_Status(404);
            res.Send(response.dump());
            return;
        }

        size_t userID = db.userIdFromName(req.path_params.at("username"));

        if (db.userHasTournamentSaved(userID, tournamentID)){
            response["message"] = "That tournament is already saved to your profile.";
            res.Set_Status(409);
            res.Send(response.dump());
            return;
        }

        if (db.tournamentExists(tournamentID)){
            db.saveTournamentToUser(userID, tournamentID);
            response["success"] = true;
            response["message"] = "OK";
            response["id"] = std::to_string(tournamentID);
            res.Send(response.dump());
            return;
        }
        else{
            response["message"] = "Sorry, that tournament doesn't exist.";
            res.Set_Status(404);
            res.Send(response.dump());
            return;
        }
    });

    app.Add_Handler("DELETE", authorizedRoute, "/tournament/:id", [=, this](const Request& req, Response& res){
        json response;
        response["success"] = false;
        size_t tournamentID = 0;
        try{
            tournamentID = stoul(req.path_params.at("id"));
        }
        catch (...){
            response["message"] = "Sorry, that tournament doesn't exist.";
            res.Set_Status(404);
            res.Send(response.dump());
            return;
        }
        if (db.tournamentExists(tournamentID)){
            size_t userID = db.userIdFromName(req.path_params.at("username"));
            db.deleteSavedTournament(userID, tournamentID);
            response["success"] = true;
            response["message"] = "OK";
            res.Send(response.dump());
            return;
        }
        else{
            response["message"] = "Sorry, that tournament doesn't exist.";
            res.Set_Status(404);
            res.Send(response.dump());
            return;
        }
    });

    app.Add_Handler("PUT", authorizedRoute, "/tournament/:id", [=, this](const Request& req, Response& res){
        json response;
        response["success"] = false;
        json request;
        try {
            request = json::parse(req.body);
        }
        catch (const json::parse_error& e){
            response["message"] = "Bad Request: " + std::string(e.what());
            res.Set_Status(400);
            res.Send(response.dump());
            return;
        }
        std::string problems = checkForString(request, "", "newName");
        if (problems != ""){
            sendProblemResponse(problems, response, res);
            return;
        }

        std::string newName = tflib::trim(request["newName"].get<std::string>());

        if (newName == ""){
            response["message"] = "Please provide a name for the tournament.";
            res.Set_Status(400);
            res.Send(response.dump());
            return;
        }

        if (newName.size() > 32){
            response["message"] = "Tournament names cannot be longer than 32 characters.";
            res.Set_Status(400);
            res.Send(response.dump());
            return;
        }

        size_t tournamentID = 0;
        try{
            tournamentID = stoul(req.path_params.at("id"));
        }
        catch (...){}

        if (!tournamentID || !db.tournamentExists(tournamentID)){
            response["message"] = "Sorry, that tournament doesn't exist.";
            res.Set_Status(404);
            res.Send(response.dump());
            return;
        }

        if (db.tournamentHasName(tournamentID, newName)){
            response["message"] = "OK";
            response["success"] = true;
            res.Send(response.dump());
            return;
        }

        bool result = db.updateTournamentName(tournamentID, req.path_params.at("username"), newName);
        if (!result){
            response["message"] = "Unauthorized: You can only change the name of tournaments you ran and you must have been logged in when you ran it.";
            res.Set_Status(401);
            res.Send(response.dump());
            return;
        }

        response["message"] = "OK";
        response["success"] = true;
        res.Send(response.dump());
    });

    app.Add_Handler("POST", baseRoute, "/user", [=, this](const Request& req, Response& res){
        json response;
        response["success"] = false;
        response["id"] = "-1";
        json request;
        try {
            request = json::parse(req.body);
        }
        catch (const json::parse_error& e){
            response["message"] = "Bad Request: " + std::string(e.what());
            res.Set_Status(400);
            res.Send(response.dump());
            return;
        }
        std::string problems = validateCreateUserRequest(request);
        if (problems != ""){
            sendProblemResponse(problems, response, res);
            return;
        }

        std::string username = tflib::trim(request["username"].get<std::string>());
        std::string password = request["password"].get<std::string>();

        if (db.userIdFromName(username)){
            response["message"] = "Sorry, that username is taken.";
            res.Set_Status(409);
            res.Send(response.dump());
            return;
        }

        if (ipSignUps[req.ip] >= maxSignUpsPerDay){
            response["message"] = "Too many sign-ups from your IP today. Please try again tomorrow.";
            res.Set_Status(429);
            res.Send(response.dump());
            return;
        }

        size_t userID = db.createUser(username, password);

        ipSignUpsMutex.lock();
        ipSignUps[req.ip]++;
        ipSignUpsMutex.unlock();

        response["success"] = true;
        response["id"] = std::to_string(userID);
        response["message"] = "OK";
        res.Send(response.dump());
    });

    app.Add_Handler("DELETE", authorizedRoute, "/", [=, this](const Request& req, Response& res){
        std::string username = req.path_params.at("username");

        bool result = db.deleteUser(username);


        json response;
        if (!result){
            response["success"] = false;
            response["message"] = "That user doesn't exist.";
            res.Send(response.dump());
            return;
        }

        response["success"] = true;
        response["message"] = "User deleted successfully.";
        res.Send(response.dump());
    });

    app.Add_Handler("GET", authorizedRoute, "/errors/count", [=, this](const Request& req, Response& res){
        json response;
        response["success"] = false;

        const User& user = db.getUserData(req.path_params.at("username")).value();
        if (!user.isAdmin){
            response["message"] = "You must be an admin to access this data.";
            res.Set_Status(401);
            res.Send(response.dump());
            return;
        }

        auto count = db.getTotalErrorBattles();

        response["data"] = count;
        response["message"] = "OK";
        response["success"] = true;
        res.Send(response.dump());
    });

    app.Add_Handler("GET", authorizedRoute, "/errors", [=, this](const Request& req, Response& res){
        json response;
        response["success"] = false;

        const User& user = db.getUserData(req.path_params.at("username")).value();
        if (!user.isAdmin){
            response["message"] = "You must be an admin to access this data.";
            res.Set_Status(401);
            res.Send(response.dump());
            return;
        }

        if (!req.query_params.contains("page") || req.query_params.at("page").size() < 1){
            response["message"] = "Bad Request: Unsupplied required query parameter: 'page'";
            res.Set_Status(400);
            res.Send(response.dump());
            return;
        }

        if (!req.query_params.contains("count") || req.query_params.at("count").size() < 1){
            response["message"] = "Bad Request: Unsupplied required query parameter: 'count'";
            res.Set_Status(400);
            res.Send(response.dump());
            return;
        }

        if (!isUnsignedInteger(req.query_params.at("page")[0])){
            response["message"] = "Bad Request: Query parameter 'page' must be a valid unsigned signed integer.";
            res.Set_Status(400);
            res.Send(response.dump());
            return;
        }

        if (!isUnsignedInteger(req.query_params.at("count")[0])){
            response["message"] = "Bad Request: Query parameter 'count' must be a valid unsigned signed integer.";
            res.Set_Status(400);
            res.Send(response.dump());
            return;
        }

        uint32_t page = stoul(req.query_params.at("page")[0]);
        uint32_t count = stoul(req.query_params.at("count")[0]);

        auto errorBattles = db.getErrorBattles(page, count);
        std::vector<json> jsons;
        jsons.reserve(errorBattles.size());
        for (auto& battle : errorBattles){
            json json;
            json["battle"] = battle.request;
            json["dateRan"] = battle.dateRan;
            json["hash"] = std::to_string(battle.hash);
            jsons.push_back(json);
        }

        response["success"] = true;
        response["message"] = "OK";
        response["data"] = jsons;
        res.Send(response.dump());
    });

    app.Add_Handler("DELETE", authorizedRoute, "/error/:id", [=, this](const Request& req, Response& res){
        json response;
        response["success"] = false;

        const User& user = db.getUserData(req.path_params.at("username")).value();
        if (!user.isAdmin){
            response["message"] = "You must be an admin to access this data.";
            res.Set_Status(401);
            res.Send(response.dump());
            return;
        }
        size_t id = 0;
        try {
            id = stoul(req.path_params.at("id"));
        }
        catch (...){
            response["message"] = "That error battle doesn't exist.";
            res.Set_Status(404);
            res.Send(response.dump());
            return;
        }

        bool deleted = db.deleteErrorBattle(id);

        if (!deleted){
            response["message"] = "That error battle doesn't exist.";
            res.Set_Status(404);
            res.Send(response.dump());
            return;
        }

        response["success"] = true;
        response["message"] = "OK";
        res.Send(response.dump());
    });

    app.Add_Handler("POST", authorizedRoute, "/error", [=, this](const Request& req, Response& res){
        json response;
        response["success"] = false;
        json request;
        try {
            request = json::parse(req.body);
        }
        catch (const json::parse_error& e){
            response["message"] = "Bad Request: " + std::string(e.what());
            res.Set_Status(400);
            res.Send(response.dump());
            return;
        }

        const User& user = db.getUserData(req.path_params.at("username")).value();
        if (!user.isAdmin){
            response["message"] = "You must be an admin to access this data.";
            res.Set_Status(401);
            res.Send(response.dump());
            return;
        }

        std::string problems = validateBattleRequest(request);
        if (problems != ""){
            sendProblemResponse(problems, response, res);
            return;
        }

        // Simulate Battle
        Trainer trainer1(request["trainer1"]);
        Trainer trainer2(request["trainer2"]);
        size_t seed = seedFromString(request["seed"].get<std::string>());
        Battle battle(trainer1, trainer2, seed);
        battle.simulate();

        response["success"] = true;
        response["data"] = battle.textLog() + "\n" + battle.errorMessage;
        response["message"] = "OK";
        res.Send(response.dump());
    });
}

int npcs::Server::run(){
    startTournamentThreads();
    app.Listen();
    return 0;
}

void npcs::Server::waitForTournaments(uint32_t threadNumber){
    try{
    auto& queue = this->queuedTournaments[threadNumber];
    auto& queueMutex = this->queuedTournamentMutexes[threadNumber];
    int tournamentsRan = 0;
    int64_t totalMsTourn = 0;
    int64_t totalMsSave = 0;
    while(true){
        //TODO: Not this
        while(queue.size() == 0){
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        queueMutex.lock();
        TournamentRequest req = queue.front();
        queueMutex.unlock();
        json& request = req.requestJson;

        std::cout << "Starting tournament " + std::to_string(req.id) + " on thread #" << threadNumber << std::endl;

        std::vector<Trainer> trainers;
        trainers.reserve(request["trainers"].size());
        for(int i = 0; i < request["trainers"].size(); i++){
            trainers.emplace_back(request["trainers"][i]);
        }
        size_t seed = seedFromString(request["seed"].get<std::string>());
        int rounds = request["rounds"].get<int>();

        std::cout << "Simulating tournament...\n";
        auto tournStart = std::chrono::high_resolution_clock::now();
        Tournament tournament(trainers, rounds, seed);
        tournament.run();
        auto tournEnd = std::chrono::high_resolution_clock::now();
        std::cout << "Done simulating " + std::to_string(req.id) + "\n";

        for (const auto& errorBattle : tournament.errorBattles){
            json battleJson;
            battleJson["trainer1"] = trainers[errorBattle.trainer1].toJSON();
            battleJson["trainer2"] = trainers[errorBattle.trainer2].toJSON();
            battleJson["seed"] = errorBattle.seed;
            db.saveErrorBattle(battleJson.dump());
        }
        
        // Save tournament to DB
        std::cout << "Saving " + std::to_string(req.id) + "\n";
        
        auto saveStart = std::chrono::high_resolution_clock::now();
        db.saveTournament(tournament, req.id);
        auto saveEnd = std::chrono::high_resolution_clock::now();
        std::cout << "Saved " + std::to_string(req.id) + "\n";
        std::unique_lock lk2(queueMutex);
        queue.pop_front();
        std::unique_lock lk(idToThreadMutex);
        idToThread.erase(req.id);
        
        int64_t tournMS = std::chrono::duration_cast<std::chrono::milliseconds>(tournEnd - tournStart).count();
        int64_t saveMS = std::chrono::duration_cast<std::chrono::milliseconds>(saveEnd - saveStart).count();
        totalMsTourn += tournMS;
        totalMsSave += saveMS;
        tournamentsRan++;
        float avgtournSeconds = (float) totalMsTourn / (float)tournamentsRan / 1000.0f;
        float avgsaveSeconds = (float) totalMsSave /(float) tournamentsRan / 1000.0f;
        
        std::cout << "Average tournament time for thread " + std::to_string(threadNumber) + ": " + std::to_string(avgtournSeconds) + " seconds\n";
        std::cout << "Average save time for thread " + std::to_string(threadNumber) + "      : " + std::to_string(avgsaveSeconds) + " seconds\n";
    }
    } catch (const std::exception& e){
        std::cerr << "ERROR: Uncaught exception on thread #" + std::to_string(threadNumber) + ":\n" + e.what() + "\nStopping.\n";
        exit(1);
    }
}

void npcs::Server::startTournamentThreads(){
    for(int i = 0; i < max_tournament_threads; i++){
        std::cout << "Starting thread #" << i << '\n';
        std::thread t(&npcs::Server::waitForTournaments, this, i);
        t.detach();
    }
}

npcs::Server::~Server(){
    delete[] queuedTournaments;
    delete[] queuedTournamentMutexes;
}

std::string npcs::Server::getDomainFromURL(){
    size_t start = websiteURL.find("//");
    if (start == std::string::npos){
        start = 0;
    }
    else{
        start += 2;
    }
    size_t end = websiteURL.find(':', start);
    size_t size = end - start;
    return websiteURL.substr(start, size);
}

bool FailedLoginState::canTryAgain(){
    int excessAttempts = numFails - MAX_ALLOWED_LOGIN_ATTEMPTS;
    if (excessAttempts > 0 && std::chrono::high_resolution_clock::now() >= nextAllowed + std::chrono::minutes(30)){
        reset();
    }
    return std::chrono::high_resolution_clock::now() >= nextAllowed;
}
void FailedLoginState::increment(){
    std::unique_lock lk(mut);
    this->numFails++;
    int excessAttempts = numFails - MAX_ALLOWED_LOGIN_ATTEMPTS;
    if (excessAttempts > 0){
        nextAllowed = std::chrono::high_resolution_clock::now() + std::chrono::minutes(1 * excessAttempts);
    }
}

void FailedLoginState::reset(){
    std::unique_lock lk(mut);
    nextAllowed = std::chrono::high_resolution_clock::now();
    numFails = 0;
}