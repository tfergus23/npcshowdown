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
#define ALLOWED_HEADERS "Authorization,Content-Type,X-Requested-With"
#define ALLOWED_METHODS "GET,POST,PUT,DELETE,OPTIONS"
const size_t MAX_REQUEST_SIZE = 524288;

const std::string SPECIES_DATA_RESPONSE = createSpeciesDataResponse();
const std::string ABILITY_DATA_RESPONSE = createAbilityDataResponse();
const std::string ITEM_DATA_RESPONSE = createItemDataResponse();
const std::string NATURE_DATA_RESPONSE = createNatureDataResponse();
const std::string MOVE_DATA_RESPONSE = createMoveDataResponse();
const std::string ALL_DATA_RESPONSE = createAllDataResponse();

bool addCORSHeaderMiddleware(const HTTP_Request& req, HTTP_Response& res){
    res.headers["Access-Control-Allow-Origin"] = "*";
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

struct TournamentResults{
    std::vector<TrainerStats> trainerStats;
    std::vector<size_t> trainers;
    bool ready = false;
};

// Mock database
std::vector<json> savedTrainers;
std::mutex saveTrainerMutex;

std::vector<BattleResult> savedBattles;
std::mutex saveBattleMutex;

std::vector<TournamentResults> savedTournaments;
std::mutex saveTournamentMutex;

size_t saveTrainer(const json& json){
    saveTrainerMutex.lock();
    savedTrainers.push_back(json);
    size_t id = savedTrainers.size() - 1;
    saveTrainerMutex.unlock();
    return id;
}

size_t saveBattle(const Trainer& trainer1, const Trainer& trainer2, size_t seed){
    size_t t1ID = saveTrainer(trainer1.toJSON());
    size_t t2ID = saveTrainer(trainer2.toJSON());
    saveBattleMutex.lock();
    savedBattles.push_back({t1ID, t2ID, seed, 0}); // The 'winner' field isn't used in this case
    size_t id = savedBattles.size() - 1;
    saveBattleMutex.unlock();
    return id;
}

size_t saveBattle(const BattleResult result){
    saveBattleMutex.lock();
    savedBattles.push_back(result);
    size_t id = savedBattles.size() - 1;
    saveBattleMutex.unlock();
    return id;
}

void saveTournament(const Tournament& tournament, size_t id){
    saveTournamentMutex.lock();
    TournamentResults result = savedTournaments.at(id);
    saveTournamentMutex.unlock();
    std::vector<size_t> trainers;
    for (auto& trainer : tournament.trainers){
        trainers.push_back(saveTrainer(trainer.toJSON()));
    }
    result.trainers = trainers;

    std::vector<TrainerStats> stats;
    std::unordered_map<size_t,size_t> addedBattles;
    for (auto stat : tournament.trainerStats){
        if (!addedBattles.contains(stat.bestWin) && stat.bestWin >= 0){
            BattleResult bestWin = tournament.results[stat.bestWin];
            bestWin.trainer1 = trainers[bestWin.trainer1];
            bestWin.trainer2 = trainers[bestWin.trainer2];
            bestWin.winner = trainers[bestWin.winner];
            addedBattles[stat.bestWin] = saveBattle(bestWin);
        }
        stat.bestWin = addedBattles[stat.bestWin];
        stats.push_back(stat);
    }
    result.trainerStats = stats;

    result.ready = true;
    
    saveTournamentMutex.lock();
    savedTournaments.at(id) = result;
    saveTournamentMutex.unlock();
}

size_t NPCS_API_Server::createTournamentRequest(const json& json){
    saveTournamentMutex.lock();
    savedTournaments.emplace_back();
    size_t id = savedTournaments.size() - 1;
    saveTournamentMutex.unlock();
    TournamentRequest request{
        .requestJson = json,
        .id = id
    };
    int threadNumber = tournamentRequestThreadCounter++;
    auto& queue = queuedTournaments[threadNumber];
    auto& mutex = queuedTournamentMutexes[threadNumber];
    tournamentRequestThreadCounter = tournamentRequestThreadCounter % MAX_TOURNAMENT_THREADS;
    mutex.lock();
    std::cout << "Queing up a tournament on thread #" << threadNumber << '\n';
    queue.push(request);
    mutex.unlock();

    return id;
}

NPCS_API_Server::NPCS_API_Server() : app{MAX_REQUEST_SIZE}{

    //Create routes
    Route* baseRoute = app.Create_Route("/api");
    Route* authorizedRoute = app.Create_Route("/api/user/:username");

    //Add middlewares
    app.base_route.Use(addCORSHeaderMiddleware);
    baseRoute->Use(addCORSHeaderMiddleware);
    authorizedRoute->Use(addCORSHeaderMiddleware);
    authorizedRoute->Use([=, this](const HTTP_Request& req, HTTP_Response& res){
        try {
            std::string token = req.headers.at("Authorization");
            std::string username = req.path_params.at("username");
            if (isTokenValid(username, token)){
                return true;
            }
            else{
                res.Set_Status(401);
                res.Send("");
                return false;
            }
        } 
        catch (...){
            res.Set_Status(400);
            res.Send("");
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
            std::string token = getToken(body["username"].get<std::string>(), body["password"].get<std::string>());
            if (token != ""){
                response["success"] = true;
                response["token"] = token;
                response["message"] = "success";
            }
            else{
                response["success"] = false;
                response["message"] = "Invalid credentials.";
                res.Set_Status(401);
            }
            res.Send(response.dump());
        } catch(...){
            res.Set_Status(400);
            res.Send("");
        }
    });

    app.Add_Handler("GET", authorizedRoute, "/", [](const HTTP_Request& req, HTTP_Response& res){
        json response;
        response["name"] = req.path_params.at("username");
        response["id"] = 0;
        response["success"] = true;
        response["token"] = "admin:123";
        res.Send(response.dump());
    });

    app.Add_Handler("PUT", authorizedRoute, "/logout", [](const HTTP_Request& req, HTTP_Response& res){
        json response;
        response["success"] = true;
        response["message"] = "Logout successful";
        res.Send(response.dump());
    });

    app.Add_Handler("GET", baseRoute, "/data/species", [](const HTTP_Request& req, HTTP_Response& res){
        res.Send(SPECIES_DATA_RESPONSE);
    });

    app.Add_Handler("GET", baseRoute, "/data", [](const HTTP_Request& req, HTTP_Response& res){
        res.Send(ALL_DATA_RESPONSE);
    });

    app.Add_Handler("POST", baseRoute, "/battle", [](const HTTP_Request& req, HTTP_Response& res){
        json response;
        response["success"] = false;
        response["id"] = -1;
        json request;
        try {
            request = json::parse(req.body);
        }
        catch (...){
            response["message"] = "Bad Request: Invalid JSON";
            res.Set_Status(400);
            res.Send(response.dump());
            return;
        }
        std::string problems = validateBattleRequest(request);
        if (problems != ""){
            response["message"] = problems;
            res.Set_Status(400);
            res.Send(response.dump());
            return;
        }

        // Simulate Battle
        Trainer trainer1(request["trainer1"]);
        Trainer trainer2(request["trainer2"]);
        std::string seedString = request["seed"].get<std::string>();
        size_t seed = seedFromString(seedString);

        // Save the battle to the databasae
        size_t id = saveBattle(trainer1, trainer2, seed);

        // Send back battle ID
        response["success"] = true;
        response["id"] = id;
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
        catch (...){
            response["message"] = "Bad Request: Invalid JSON";
            res.Set_Status(400);
            res.Send(response.dump());
            return;
        }
        std::string problems = validateTournamentRequest(request);
        if (problems != ""){
            response["message"] = problems;
            res.Set_Status(400);
            res.Send(response.dump());
            return;
        }

        size_t id  = createTournamentRequest(request);

        // Send back tournament ID
        response["success"] = true;
        response["id"] = id;
        response["message"] = "OK";
        res.Send(response.dump());
    });

    app.Add_Handler("GET", baseRoute, "/battle/:id", [](const HTTP_Request& req, HTTP_Response& res){
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
            BattleResult br = savedBattles.at(battleID);
            Trainer trainer1(savedTrainers[br.trainer1]);
            Trainer trainer2(savedTrainers[br.trainer2]);
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

    app.Add_Handler("GET", baseRoute, "/tournament/:id", [](const HTTP_Request& req, HTTP_Response& res){
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
            const TournamentResults& tr = savedTournaments.at(tournamentID);
            if (!tr.ready){
                response["message"] = "Please wait. Your tournament is in queue...";
                response["success"] = false;
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
                trainerJSONs.push_back(savedTrainers.at(trainer));
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

bool NPCS_API_Server::isTokenValid(const std::string& username, const std::string& token){
    //Get username token from database
    //Make sure the token matches the one provided
    return username == "admin" && token == "admin:123";
}

void NPCS_API_Server::waitForTournaments(uint32_t threadNumber){
    try{
    auto& queue = this->queuedTournaments.at(threadNumber);
    auto& queueMutex = this->queuedTournamentMutexes.at(threadNumber);
    while(true){
        //TODO: Not this
        while(queue.size() == 0){
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        std::cout << "Starting tournament on thread #" << threadNumber << '\n';
        queueMutex.lock();
        TournamentRequest req = queue.front();
        json& request = req.requestJson;
        queue.pop();
        queueMutex.unlock();

        std::vector<Trainer> trainers;
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
        saveTournament(tournament, req.id);
    }
    } catch (const std::exception& e){
        std::cout << e.what() << '\n';
        exit(1);
    }
}

void NPCS_API_Server::startTournamentThreads(){
    for(int i = 0; i < MAX_TOURNAMENT_THREADS; i++){
        std::cout << "Starting thread #" << i << '\n';
        std::thread t(&NPCS_API_Server::waitForTournaments, this, i);
        t.detach();
    }
}
