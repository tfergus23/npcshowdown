#include "api/NPCS_API_Server.hpp"
#include "nlohmann/json.hpp"
#include "sim/battle/Battle.hpp"
#include "sim/data/Abilities.hpp"
#include "sim/data/Moves.hpp"
#include <chrono>
#include <thread>
#include "api/api_utils.hpp"
#include "sim/tournament/Tournament.hpp"
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include "tflib/strings.h"


using namespace tfhttp;
using json = nlohmann::json;

#define ALLOWED_HEADERS "Authorization,Content-Type,X-Requested-With,Set-Cookie"
#define ALLOWED_METHODS "GET,POST,PUT,DELETE,OPTIONS"


void preflightHandler(const HTTP_Request& req, HTTP_Response& res){
    res.headers["Access-Control-Allow-Methods"] = ALLOWED_METHODS;
    res.headers["Access-Control-Allow-Headers"] = ALLOWED_HEADERS;
    res.Send("");
}

bool debugDelayMiddleware(const HTTP_Request& req, HTTP_Response& res){
    std::this_thread::sleep_for(std::chrono::seconds(1));
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

size_t NPCS_API_Server::createTournamentRequest(const json& json, size_t user){
    size_t id = db.createEmptyTournament(user);
    TournamentRequest request{
        .requestJson = json,
        .id = id,
        .user = user
    };
    threadCounterMutex.lock();
    int threadNumber = tournamentRequestThreadCounter++;
    auto& queue = queuedTournaments[threadNumber];
    auto& mutex = queuedTournamentMutexes[threadNumber];
    tournamentRequestThreadCounter = tournamentRequestThreadCounter % max_tournament_threads;
    threadCounterMutex.unlock();
    mutex.lock();
    idToThreadMutex.lock();
    idToThread[id] = threadNumber;
    idToThreadMutex.unlock();
    std::cout << "Queing up a tournament on thread #" << threadNumber << '\n';
    queue.push_back(request);
    mutex.unlock();

    return id;
}

int NPCS_API_Server::findTournamentPositionInQueue(size_t tournamentID, int threadNumber){
    auto& queue = queuedTournaments[threadNumber];
    int pos = 0;
    for(const auto& request : queue){
        if (request.id == tournamentID){
            return pos;
        }
        pos++;
    }
    return -1;
}

const std::string TOKEN_COOKIE_START = "token=";

std::string getTokenFromRequest(const HTTP_Request& req){
    if (req.headers.contains("Cookie")){
        const std::string& cookieHeader = req.headers.at("Cookie");
        size_t cookieBegin = cookieHeader.find(TOKEN_COOKIE_START) + TOKEN_COOKIE_START.size();
        return cookieHeader.substr(cookieBegin, 32);
    }
    else if (req.headers.contains("Authorization")){
        return req.headers.at("Authorization");
    }
    else{
        return "";
    }
}

NPCS_API_Server::NPCS_API_Server() :
 SPECIES_DATA_RESPONSE{createSpeciesDataResponse()},
 ABILITY_DATA_RESPONSE{createAbilityDataResponse()},
 ITEM_DATA_RESPONSE{createItemDataResponse()},
 NATURE_DATA_RESPONSE{createNatureDataResponse()},
 MOVE_DATA_RESPONSE{createMoveDataResponse()},
 ALL_DATA_RESPONSE{createAllDataResponse()}
 {
    max_tournament_threads = getIntFromConfig(config, "tournament_threads");
    max_trainers_per_user = getIntFromConfig(config, "max_trainers_per_user");

    if (max_tournament_threads < 1){
        throw std::runtime_error("Invalid max tournament threads in ini file: " + config.get("tournament_threads"));
    }

    if (max_trainers_per_user < 1){
        throw std::runtime_error("Invalid max trainers per user in ini file: " + config.get("max_trainers_per_user"));
    }

    queuedTournaments = new std::deque<TournamentRequest>[max_tournament_threads];
    queuedTournamentMutexes = new std::mutex[max_tournament_threads];

    //Create routes
    Route* baseRoute = app.Create_Route("/api");
    Route* authorizedRoute = app.Create_Route("/api/user/:username");

    auto addCORSHeaderMiddleware = [=, this](const HTTP_Request& req, HTTP_Response& res){
        res.headers["Access-Control-Allow-Origin"] = websiteURL;
        res.headers["Access-Control-Allow-Credentials"] = "true";
        return true;
    };

    //Add middlewares
    app.base_route.Use(addCORSHeaderMiddleware);
    baseRoute->Use(addCORSHeaderMiddleware);
    authorizedRoute->Use(addCORSHeaderMiddleware);
    authorizedRoute->Use([=, this](const HTTP_Request& req, HTTP_Response& res){
        json response;
        response["success"] = false;
        std::string username = req.path_params.at("username");

        std::string token = getTokenFromRequest(req);
        if (db.isTokenValid(username, token)){
            db.updateTokenLastUsed(username, token);
            res.headers["Set-Cookie"] = "token=" + token + "; Max-Age=2147483647; HttpOnly; Secure; Path=/; SameSite=Strict; Domain=" + domain;
            return true;
        }
        else{
            response["message"] = "Unauthorized: Invalid token.";
            res.Set_Status(401);
            res.Send(response.dump());
            return false;
        }

    });

    // Fake delay for debugging
    //baseRoute->Use(debugDelayMiddleware);
    //authorizedRoute->Use(debugDelayMiddleware);

    //Add preflight handler to all paths
    app.Add_Handler("OPTIONS", "*", preflightHandler);

    //Add API handlers 
    app.Add_Handler("POST", baseRoute, "/auth", [=, this](const HTTP_Request& req, HTTP_Response& res){
        json response;
        try {
            json body = json::parse(req.body);
            std::string problems = validateAuthRequestSchema(body);
            if (problems != ""){
                if (problems[problems.size() - 1] == '\n'){
                    problems.pop_back();
                }
                response["message"] = problems;
                response["success"] = false;
                res.Set_Status(400);
                res.Send(response.dump());
                return;
            }
            std::string token;
            problems = db.createUserSession(body["username"], body["password"], token);

            if (problems != ""){
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

        } 
        catch (const json::parse_error& e){
            response["success"] = false;
            response["message"] = "Bad Request: " + std::string(e.what());
            res.Set_Status(400);
            res.Send(response.dump());
            return;
        }
    });

    app.Add_Handler("GET", authorizedRoute, "/", [=, this](const HTTP_Request& req, HTTP_Response& res){
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
        data["id"] = user.id;
        data["name"] = user.name;
        data["accountCreated"] = user.accountCreated;
        data["lastPasswordChange"] = user.lastPasswordChange;
        response["success"] = true;
        response["message"] = "OK";
        response["data"] = data;
        res.Send(response.dump());
    });

    app.Add_Handler("DELETE", authorizedRoute, "/logout", [=, this](const HTTP_Request& req, HTTP_Response& res){
        db.deleteUserSession(req.path_params.at("username"), getTokenFromRequest(req));
        json response;
        response["success"] = true;
        response["message"] = "OK";
        res.Send(response.dump());
    });

    app.Add_Handler("PUT", authorizedRoute, "/password", [=, this](const HTTP_Request& req, HTTP_Response& res){
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
            if (problems[problems.size() - 1] == '\n'){
                problems.pop_back();
            }
            response["message"] = problems;
            res.Set_Status(400);
            res.Send(response.dump());
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

    app.Add_Handler("GET", baseRoute, "/data/species", [=,this](const HTTP_Request& req, HTTP_Response& res){
        res.Send(SPECIES_DATA_RESPONSE);
    });

    app.Add_Handler("GET", baseRoute, "/data", [=,this](const HTTP_Request& req, HTTP_Response& res){
        res.Send(ALL_DATA_RESPONSE);
    });

    app.Add_Handler("POST", baseRoute, "/battle", [=, this](const HTTP_Request& req, HTTP_Response& res){
        json response;
        response["success"] = false;
        response["id"] = -1;
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
            if (problems[problems.size() - 1] == '\n'){
                problems.pop_back();
            }
            response["message"] = problems;
            res.Set_Status(400);
            res.Send(response.dump());
            return;
        }

        // Simulate Battle
        Trainer trainer1(request["trainer1"]);
        Trainer trainer2(request["trainer2"]);
        size_t seed = seedFromString(request["seed"].get<std::string>());
        Battle battle(trainer1, trainer2, seed);
        battle.simulate();

        // Send back battle ID
        response["success"] = true;
        response["data"] = battle.battleLog;
        response["message"] = "OK";
        res.Send(response.dump());
    });
    
    app.Add_Handler("POST", baseRoute, "/tournament", [=,this](const HTTP_Request& req, HTTP_Response& res){
        json response;
        response["success"] = false;
        response["id"] = -1;
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
            if (problems[problems.size() - 1] == '\n'){
                problems.pop_back();
            }
            response["message"] = problems;
            res.Set_Status(400);
            res.Send(response.dump());
            return;
        }
        size_t userID = 0;
        if (request.contains("user")){
            std::string username = request["user"].get<std::string>();
            std::string token = getTokenFromRequest(req);
            if (db.isTokenValid(username, token)){
                db.updateTokenLastUsed(username, token);
                userID = db.userIdFromName(username);
            }
            else{
                response["message"] = "Unauthorized: Invalid token.";
                res.Set_Status(401);
                res.Send(response.dump());
                return;
            }
        }

        size_t id  = createTournamentRequest(request, userID);

        // Send back tournament ID
        response["success"] = true;
        response["id"] = id;
        response["message"] = "OK";
        res.Send(response.dump());
    });

    app.Add_Handler("GET", baseRoute, "/trainer/:id", [=, this](const HTTP_Request& req, HTTP_Response& res){
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

    app.Add_Handler("GET", baseRoute, "/tournament/:id", [=,this](const HTTP_Request& req, HTTP_Response& res){
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
            int position = findTournamentPositionInQueue(tournamentID, threadNumber);
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
                        response["data"] = tr.toJSON(db);
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
                    response["data"] = tr.toJSON(db);
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

    app.Add_Handler("GET", authorizedRoute, "/trainers", [=, this](const HTTP_Request& req, HTTP_Response& res){
        std::string username = req.path_params.at("username");

        std::vector<json> userTrainerJSONs = db.getUserTrainers(username);

        json response;
        response["success"] = true;
        response["message"] = "OK";
        response["data"] = userTrainerJSONs;

        res.Send(response.dump());
    });

    app.Add_Handler("POST", authorizedRoute, "/trainer", [=, this](const HTTP_Request& req, HTTP_Response& res){
        json response;
        response["success"] = false;
        response["id"] = -1;
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
            if (problems[problems.size() - 1] == '\n'){
                problems.pop_back();
            }
            response["message"] = problems;
            response["success"] = false;
            res.Set_Status(400);
            res.Send(response.dump());
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
        response["id"] = trainerID;
        res.Send(response.dump());
        return;

    });

    app.Add_Handler("PUT", authorizedRoute, "/trainer/:id", [=, this](const HTTP_Request& req, HTTP_Response& res){
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
            if (problems[problems.size() - 1] == '\n'){
                problems.pop_back();
            }
            response["message"] = problems;
            response["success"] = false;
            res.Set_Status(400);
            res.Send(response.dump());
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

    app.Add_Handler("DELETE", authorizedRoute, "/trainer/:id", [=, this](const HTTP_Request& req, HTTP_Response& res){
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

    app.Add_Handler("GET", authorizedRoute, "/tournaments", [=, this](const HTTP_Request& req, HTTP_Response& res){
        std::vector<TournamentResults> savedTournaments = db.getUserTournaments(req.path_params.at("username"));
        std::vector<json> savedTournamentJSONs;
        savedTournamentJSONs.reserve(savedTournaments.size());

        for (auto& tournament : savedTournaments){
            savedTournamentJSONs.push_back(tournament.toJSON(db));
        }

        json response;
        response["success"] = true;
        response["message"] = "OK";
        response["data"] = savedTournamentJSONs;

        res.Send(response.dump());
    });

    app.Add_Handler("POST", authorizedRoute, "/tournament/:id", [=, this](const HTTP_Request& req, HTTP_Response& res){
        json response;
        response["success"] = false;
        response["id"] = -1;

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
            response["id"] = tournamentID;
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

    app.Add_Handler("DELETE", authorizedRoute, "/tournament/:id", [=, this](const HTTP_Request& req, HTTP_Response& res){
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

    app.Add_Handler("POST", baseRoute, "/user", [=, this](const HTTP_Request& req, HTTP_Response& res){
        json response;
        response["success"] = false;
        response["id"] = -1;
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
            if (problems[problems.size() - 1] == '\n'){
                problems.pop_back();
            }
            response["message"] = problems;
            res.Set_Status(400);
            res.Send(response.dump());
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

        size_t userID = db.createUser(username, password);

        response["success"] = true;
        response["id"] = userID;
        response["message"] = "OK";
        res.Send(response.dump());
    });

    app.Add_Handler("DELETE", authorizedRoute, "/", [=, this](const HTTP_Request& req, HTTP_Response& res){
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

}

int NPCS_API_Server::run(){
    startTournamentThreads();
    app.Listen(port);
    return 0;
}

std::string NPCS_API_Server::getToken(const std::string& username, const std::string& password){
    if (username == "admin" && password  == "admin"){
        return "admin:123";
    }
    return "";
}

void NPCS_API_Server::waitForTournaments(uint32_t threadNumber){
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

void NPCS_API_Server::startTournamentThreads(){
    for(int i = 0; i < max_tournament_threads; i++){
        std::cout << "Starting thread #" << i << '\n';
        std::thread t(&NPCS_API_Server::waitForTournaments, this, i);
        t.detach();
    }
}

NPCS_API_Server::~NPCS_API_Server(){
    delete[] queuedTournaments;
    delete[] queuedTournamentMutexes;
}

void NPCS_API_Server::testTrainerSerialization(){
    std::vector<PokemonBlueprint> team = {
        {
            "Squirtle",
            100,
            {"Tackle","Tackle","Tackle","Tackle"},
            "Torrent",
            "Random",
            {31,31,31,31,31,31},
            "Adamant",
            "Leftovers",
            {255,255,255,255,255,255},
            "Squirty"
        },
        {
            "Charmander",
            99,
            {"Pound","Tackle","Karate Chop","Surf"},
            "Guts",
            "Female",
            {31,31,31,31,31,31},
            "Docile",
            "Leftovers",
            {255,255,255,255,255,3},
            "Kalameet"
        },
    };
    std::string name = "Test Guy";
    TrainerLevel level = TrainerLevel::TRAINER;

    Trainer t(name, team, level);

    size_t id = db.saveTrainer(t, 1, 0);

    Trainer tdb = db.getTrainer(id).value();
    Trainer tjs(t.toJSON());

    std::cout << t.toJSON().dump() << '\n';
    std::cout << tdb.toJSON().dump() << '\n';
    std::cout << tjs.toJSON().dump() << '\n';

    assert(t.hashCode() == tdb.hashCode());
    assert(tjs.hashCode() == tdb.hashCode());
    std::cout << t.hashCode() << '\n';
}

std::string NPCS_API_Server::getDomainFromURL(){
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