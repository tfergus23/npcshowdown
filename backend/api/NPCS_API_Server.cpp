#include "api/NPCS_API_Server.hpp"
#include "nlohmann/json.hpp"
#include "sim/data/Species.hpp"
#include <chrono>
#include <thread>

using json = nlohmann::json;

#define PORT 3000
#define WEBSITE_URL "http://localhost:4200"
#define ALLOWED_HEADERS "Authorization,Content-Type,X-Requested-With"
#define ALLOWED_METHODS "GET,POST,PUT,DELETE,OPTIONS"

bool addCORSHeaderMiddleware(const HTTP_Request& req, HTTP_Response& res){
    res.headers["Access-Control-Allow-Origin"] = "*";
    return true;
}

void preflightHandler(const HTTP_Request& req, HTTP_Response& res){
    res.headers["Access-Control-Allow-Methods"] = ALLOWED_METHODS;
    res.headers["Access-Control-Allow-Headers"] = ALLOWED_HEADERS;
    res.Send("");
}

bool authMiddleware(const HTTP_Request& req, HTTP_Response& res){
    return true;
}

NPCS_API_Server::NPCS_API_Server() : m_SpeciesData{getSpeciesData()}{
    //Create routes
    Route* baseRoute = app.Create_Route("/api");
    Route* authorizedRoute = app.Create_Route("/api/user/:username");

    //Add middlewares
    app.base_route.Use(addCORSHeaderMiddleware);
    baseRoute->Use(addCORSHeaderMiddleware);
    authorizedRoute->Use(addCORSHeaderMiddleware);
    authorizedRoute->Use(authMiddleware);

    //Add preflight handler to all paths
    app.Add_Handler("OPTIONS", "*", preflightHandler);

    //Add API handlers 
    app.Add_Handler("GET", authorizedRoute, "/", [](const HTTP_Request& req, HTTP_Response& res) {
        res.Send("Authorized route!");
    });

    app.Add_Handler("GET", baseRoute, "/", [](const HTTP_Request& req, HTTP_Response& res) {
        res.Send("Base route!");
    }); 
}

int NPCS_API_Server::run(){
    app.Listen(8080);
    return 0;
}

std::string NPCS_API_Server::authenticateUser(const std::string& username, const std::string& password){
    if (username == "BilboSwaggins" && password == "yourmom") return "BilboSwaggins:abc";
    return "";
}


bool NPCS_API_Server::isTokenValid(const std::string token){
    return token == "BilboSwaggins:abc";
}

std::string NPCS_API_Server::getSpeciesData(){
    //TODO: Sort this properly
    std::vector<std::string> result;
    for(auto [species,ptr] : speciesMap){
        result.push_back(species);
    }
    json resultJSON = result;
    return resultJSON.dump();
}
std::string getAbilityData();
std::string getItemData();
std::string getMoveData();

std::string usernameFromToken(const std::string& token){
    //TODO: This needs to lookup the token in the database and return the user it belongs to.
    std::string username("");
    for (char c : token){
        if (c == ':') break;
        username.push_back(c);
    }
    return username;
}