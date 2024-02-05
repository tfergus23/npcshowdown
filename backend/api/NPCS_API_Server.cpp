#include "api/NPCS_API_Server.hpp"
#include "nlohmann/json.hpp"
#include "sim/battle/Battle.hpp"
#include "sim/data/Abilities.hpp"
#include "sim/data/Moves.hpp"
#include <chrono>
#include <thread>

using namespace tfhttp;
using json = nlohmann::json;

#define PORT 3000
#define WEBSITE_URL "http://localhost:4200"
#define ALLOWED_HEADERS "Authorization,Content-Type,X-Requested-With"
#define ALLOWED_METHODS "GET,POST,PUT,DELETE,OPTIONS"

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

    response["success"] = true;
    response["message"] = "OK";
    response["data"] = {
        {"speciesList", speciesResponse["data"].get<std::vector<std::string>>()},
        {"abilityList", abilityResponse["data"].get<std::vector<std::string>>()},
        {"itemList", itemResponse["data"].get<std::vector<std::string>>()},
        {"natureList", natureResponse["data"].get<std::vector<std::string>>()},
        {"moveList", moveResponse["data"].get<std::vector<std::string>>()},
    };
    return response.dump();
}


NPCS_API_Server::NPCS_API_Server(){
    //Create routes
    Route* baseRoute = app.Create_Route("/api");
    Route* authorizedRoute = app.Create_Route("/api/user/:username");

    //Add middlewares
    app.base_route.Use(addCORSHeaderMiddleware);
    baseRoute->Use(addCORSHeaderMiddleware);
    authorizedRoute->Use(addCORSHeaderMiddleware);
    authorizedRoute->Use([=](const HTTP_Request& req, HTTP_Response& res){
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
    app.Add_Handler("POST", baseRoute, "/auth", [=](const HTTP_Request& req, HTTP_Response& res){
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