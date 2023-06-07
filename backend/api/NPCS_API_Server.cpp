#include "api/NPCS_API_Server.hpp"
#include "nlohmann/json.hpp"
#include "sim/data/Species.hpp"

using json = nlohmann::json;

#define PORT 3000
#define WEBSITE_URL "http://localhost:4200"

NPCS_API_Server::NPCS_API_Server() : m_Expresscpp{std::make_shared<expresscpp::ExpressCpp>()}, m_SpeciesData{getSpeciesData()}{
    auto headerMiddleWare = [&](auto req, auto res, auto next) {
        res->res.set("Access-Control-Allow-Origin", WEBSITE_URL);
        next();
    };
    m_Expresscpp->Use(headerMiddleWare);
    auto router = m_Expresscpp->GetRouter("api");
    m_Expresscpp->Use("/api", router);
    router->Post("/auth", [=](auto req, auto res) {
        json response;
        try{
            json body = json::parse(req->getBody());
            std::string username = body["username"].get<std::string>();
            std::string password = body["password"].get<std::string>();
            std::string token = authorizeUser(username, password);
            if (token != ""){
                response["success"] = true;
                response["token"] = token;
            }
            else{
                response["success"] = false;
                response["message"] = "Invalid credentials";
                res->SetStatus(401);
            }
        }
        catch (std::exception& e){
            response["success"] = false;
            response["message"] = "Bad request";
            res->SetStatus(400);
        }
        res->Send(response.dump());
    });
    router->Get("/user/:username", [](auto req, auto res) {
        res->Json("{\"name\": \"" + req->GetParams().at("username") + "\", \"id\": 1}");
    });
    router->Get("/sim/tournament/:id", [](auto req, auto res) {res->Send("WIP!");});
    router->Get("/sim/battle/:id", [](auto req, auto res) {res->Send("WIP!");});
    router->Post("/sim/tournament", [](auto req, auto res) {res->Send("WIP!");});
    router->Post("/sim/battle", [](auto req, auto res) {res->Send("WIP!");});
}

int NPCS_API_Server::run(){
    m_Expresscpp->Listen(PORT,[=](auto /*ec*/) { std::cout << "Listening on port " << PORT << "\n\n"; }).Run();
    return 0;
}

std::string NPCS_API_Server::authorizeUser(const std::string& username, const std::string& password){
    if (username == "BilboSwaggins" && password == "yourmom") return "abc";
    return "";
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