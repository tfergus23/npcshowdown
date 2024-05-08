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

std::vector<std::string> mostRecentBattleLogs;
std::vector<TournamentTrainer> mostRecentTournamentTrainers;
std::vector<Trainer> mostRecentTrainers;

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
        mostRecentBattleLogs.clear();
        Trainer trainer1(request["trainer1"]);
        Trainer trainer2(request["trainer2"]);
        std::string seedString = request["seed"].get<std::string>();
        size_t seed = seedFromString(seedString);
        std::cout << "Simulating battle...\n";
        Battle battle(trainer1, trainer2, seed);
        battle.simulate();
        std::cout << "Done simulating battle.\n";

        // Save the battle to the databasae
        mostRecentBattleLogs.push_back(battle.battleLog);

        // Send back battle ID
        response["success"] = true;
        response["id"] = 0;
        response["message"] = "OK";
        res.Send(response.dump());
    });
    
    app.Add_Handler("POST", baseRoute, "/tournament", [](const HTTP_Request& req, HTTP_Response& res){
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
        
        // Create tournament record
        mostRecentBattleLogs.clear();
        mostRecentTournamentTrainers = tournament.trainers;
        mostRecentTrainers = trainers;

        // Save upset battles to the database
        std::unordered_map<int,int> battleIDToIndex;
        int indexCounter = 0;

        for(auto& trainer : mostRecentTournamentTrainers){
            if (trainer.bestWin < 0) continue;
            if (!battleIDToIndex.contains(trainer.bestWin)){
                battleIDToIndex[trainer.bestWin] = indexCounter++;

                BattleResult& br = tournament.results[trainer.bestWin];
                const Trainer& t1 = mostRecentTrainers[tournament.trainers[br.trainer1].trainerIndex];
                const Trainer& t2 = mostRecentTrainers[tournament.trainers[br.trainer2].trainerIndex];
                size_t seed = br.seed;
                Battle battle(t1, t2, seed);
                battle.simulate();
                mostRecentBattleLogs.push_back(battle.battleLog);
            }
            trainer.bestWin = battleIDToIndex[trainer.bestWin];
        }

        // Save tournament trainers to the database

        // Send back tournament ID
        response["success"] = true;
        response["id"] = 1;
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
            response["success"] = true;
            response["message"] = "OK";
            response["data"] = mostRecentBattleLogs[battleID];
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

        if (tournamentID != 1){
            response["message"] = "Sorry, that tournament doesn't exist.";
            res.Set_Status(404);
            res.Send(response.dump());
            return;
        }
        json data;
        std::vector<json> trainerJSONs;
        std::vector<json> resultJSONs;
        for(auto& result : mostRecentTournamentTrainers){
            resultJSONs.push_back(result.toJSON());
        }
        for(auto& trainer : mostRecentTrainers){
            trainerJSONs.push_back(trainer.toJSON());
        }
        data["trainers"] = trainerJSONs;
        data["results"] = resultJSONs;
        response["success"] = true;
        response["data"] = data;
        res.Send(response.dump());
    });
}

int NPCS_API_Server::run(){
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