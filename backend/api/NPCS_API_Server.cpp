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


using namespace tfhttp;
using json = nlohmann::json;

#define PORT 3000
#define WEBSITE_URL "http://localhost:4200"
#define ALLOWED_HEADERS "Authorization,Content-Type,X-Requested-With,Set-Cookie"
#define ALLOWED_METHODS "GET,POST,PUT,DELETE,OPTIONS"
const size_t MAX_REQUEST_SIZE = 524288;

const std::string SPECIES_DATA_RESPONSE = createSpeciesDataResponse();
const std::string ABILITY_DATA_RESPONSE = createAbilityDataResponse();
const std::string ITEM_DATA_RESPONSE = createItemDataResponse();
const std::string NATURE_DATA_RESPONSE = createNatureDataResponse();
const std::string MOVE_DATA_RESPONSE = createMoveDataResponse();
const std::string ALL_DATA_RESPONSE = createAllDataResponse();

bool addCORSHeaderMiddleware(const HTTP_Request& req, HTTP_Response& res){
    res.headers["Access-Control-Allow-Origin"] = WEBSITE_URL;
    res.headers["Access-Control-Allow-Credentials"] = "true";
    return true;
}

void preflightHandler(const HTTP_Request& req, HTTP_Response& res){
    res.headers["Access-Control-Allow-Methods"] = ALLOWED_METHODS;
    res.headers["Access-Control-Allow-Headers"] = ALLOWED_HEADERS;
    res.Send("");
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
    size_t id = db.createEmptyTournament();
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
    std::cout << "Queing up a tournament on thread #" << threadNumber << '\n';
    idToThreadMutex.lock();
    idToThread[id] = threadNumber;
    idToThreadMutex.unlock();
    queue.push_back(request);
    mutex.unlock();

    return id;
}

int NPCS_API_Server::findTournamentPositionInQueue(size_t tournamentID){
    idToThreadMutex.lock();
    int threadNumber = idToThread.at(tournamentID);
    idToThreadMutex.unlock();
    auto& queue = queuedTournaments[threadNumber];
    auto& mutex = queuedTournamentMutexes[threadNumber];

    mutex.lock();
    int pos = 0;
    for(const auto& request : queue){
        if (request.id == tournamentID){
            mutex.unlock();
            return pos;
        }
        pos++;
    }
    mutex.unlock();
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

NPCS_API_Server::NPCS_API_Server() : app{MAX_REQUEST_SIZE}{
    max_tournament_threads = getIntFromConfig(config, "tournament_threads");

    if (max_tournament_threads < 1){
        throw std::runtime_error("Invalid max tournament threads in ini file: " + config.get("tournament_threads"));
    }

    queuedTournaments = new std::deque<TournamentRequest>[max_tournament_threads];
    queuedTournamentMutexes = new std::mutex[max_tournament_threads];

    //Create routes
    Route* baseRoute = app.Create_Route("/api");
    Route* authorizedRoute = app.Create_Route("/api/user/:username");

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
            res.headers["Set-Cookie"] = "token=" + token + "; Max-Age=2147483647; HttpOnly; Secure; Path=/; SameSite=Strict; Domain=localhost";
            return true;
        }
        else{
            response["message"] = "Unauthorized: Invalid token.";
            res.Set_Status(401);
            res.Send(response.dump());
            return false;
        }

    });

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
                    problems = problems.substr(0,problems.size()-1);
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

            res.headers["Set-Cookie"] = "token=" + token + "; Max-Age=2147483647; HttpOnly; Secure; Path=/; SameSite=Strict; Domain=localhost";

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

    app.Add_Handler("GET", authorizedRoute, "/", [](const HTTP_Request& req, HTTP_Response& res){
        json response;
        json data;
        data["id"] = 0;
        data["name"] = req.path_params.at("username");
        data["accountCreated"] = "2024-10-04";
        data["lastPasswordChange"] = "2024-10-04";
        data["email"] = "testperson@bmail.net";
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

    app.Add_Handler("GET", baseRoute, "/data/species", [](const HTTP_Request& req, HTTP_Response& res){
        res.Send(SPECIES_DATA_RESPONSE);
    });

    app.Add_Handler("GET", baseRoute, "/data", [](const HTTP_Request& req, HTTP_Response& res){
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
                problems = problems.substr(0,problems.size()-1);
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
                problems = problems.substr(0,problems.size()-1);
            }
            response["message"] = problems;
            res.Set_Status(400);
            res.Send(response.dump());
            return;
        }
        size_t userID = 0;
        if (request.contains("user")){
            userID = db.userIdFromName(request["user"].get<std::string>());
        }

        size_t id  = createTournamentRequest(request, userID);

        // Send back tournament ID
        response["success"] = true;
        response["id"] = id;
        response["message"] = "OK";
        res.Send(response.dump());
    });

    app.Add_Handler("GET", baseRoute, "/battle/:id", [=, this](const HTTP_Request& req, HTTP_Response& res){
        json response;
        response["success"] = false;
        size_t battleID = 0;
        try{
            battleID = stoul(req.path_params.at("id"));
        }
        catch (...){
            response["message"] = "Sorry, that battle doesn't exist.";
            res.Set_Status(404);
            res.Send(response.dump());
            return;
        }

        // Look up battle in DB
        try{
            BattleResult br = db.getBattle(battleID);
            Trainer trainer1(db.getTrainer(br.trainer1));
            Trainer trainer2(db.getTrainer(br.trainer2));
            size_t seed = br.seed;
            std::cout << "Creating battle...\n";
            Battle battle(trainer1, trainer2, seed);
            std::cout << "Done.\nSimulating battle...\n";
            battle.simulate();
            std::cout << "Done.\n";

            response["success"] = true;
            response["message"] = "OK";
            response["data"] = battle.battleLog;
            res.Send(response.dump());
        }
        catch(...){
            response["message"] = "Sorry, that battle doesn't exist.";
            res.Set_Status(404);
            res.Send(response.dump());
            return;
        }

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
        try{
            TournamentResults tr = db.getTournament(tournamentID);
            if (!tr.ready){
                int position = findTournamentPositionInQueue(tournamentID);
                if (position > 0){
                    response["message"] = "Please wait. Your tournament is in queue at position " + std::to_string(position) + ".";
                }
                else{
                    response["message"] = "Please wait. Your tournament is currently being simulated.";
                }
                response["success"] = true; // TODO: This should probably be false
                res.Set_Status(404); // TODO: What code should this be?
                res.Send(response.dump());
                return;
            }
            json data;
            std::vector<json> trainerJSONs;
            std::vector<json> statJSONs;
            for(auto& stat : tr.trainerStats){
                statJSONs.push_back(stat.toJSON());
            }
            for(auto trainer : tr.trainers){
                trainerJSONs.push_back(db.getTrainer(trainer).toJSON());
            }
            data["trainers"] = trainerJSONs;
            data["results"] = statJSONs;
            response["success"] = true;
            response["data"] = data;
            res.Send(response.dump());
        }
        catch (...){
            response["message"] = "Sorry, that tournament doesn't exist.";
            res.Set_Status(404);
            res.Send(response.dump());
            return;
        }
    });

    app.Add_Handler("GET", authorizedRoute, "/trainers", [=, this](const HTTP_Request& req, HTTP_Response& res){
        std::string username = req.path_params.at("username");


        //TODO: Do this better
        std::vector<size_t> userTrainerIDs = db.getUserTrainers(username);
        std::vector<json> userTrainers;
        userTrainers.reserve(userTrainerIDs.size());

        for(auto id : userTrainerIDs){
            userTrainers.push_back(db.getTrainer(id).toJSON());
        }

        json response;
        response["success"] = true;
        response["message"] = "OK";
        response["data"] = userTrainers;

        res.Send(response.dump());
    });  
}

int NPCS_API_Server::run(){
    startTournamentThreads();
    app.Listen(PORT);
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
    while(true){
        //TODO: Not this
        while(queue.size() == 0){
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        std::cout << "Starting tournament on thread #" << threadNumber << std::endl;
        queueMutex.lock();
        TournamentRequest req = queue.front();
        json& request = req.requestJson;
        queue.pop_front();
        queueMutex.unlock();

        std::vector<Trainer> trainers;
        trainers.reserve(request["trainers"].size());
        for(int i = 0; i < request["trainers"].size(); i++){
            trainers.emplace_back(request["trainers"][i]);
        }
        size_t seed = seedFromString(request["seed"].get<std::string>());
        int rounds = request["rounds"].get<int>();

        std::cout << "Simulating tournament...\n";
        Tournament tournament(trainers, rounds, seed);
        tournament.run();
        std::cout << "Done simulating tournament.\n";
        
        // Save tournament to DB
        db.saveTournament(tournament, req.user, req.id);
        idToThreadMutex.lock();
        idToThread.erase(req.id);
        idToThreadMutex.unlock();
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

    size_t id = db.saveTrainer(t, 0, 0);

    Trainer tdb = db.getTrainer(id);
    Trainer tjs(t.toJSON());

    std::cout << t.toJSON().dump() << '\n';
    std::cout << tdb.toJSON().dump() << '\n';
    std::cout << tjs.toJSON().dump() << '\n';

    assert(t.equals(tdb));
    assert(tjs.equals(tdb));
}