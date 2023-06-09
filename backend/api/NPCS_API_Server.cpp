#include "api/NPCS_API_Server.hpp"
#include "nlohmann/json.hpp"
#include "sim/data/Species.hpp"

using json = nlohmann::json;

#define PORT 3000
#define WEBSITE_URL "http://localhost:4200"
#define ALLOWED_HEADERS "Authorization,Content-Type"

NPCS_API_Server::NPCS_API_Server() : m_Expresscpp{std::make_shared<expresscpp::ExpressCpp>()}, m_SpeciesData{getSpeciesData()}{
    auto headerMiddleWare = [&](auto req, auto res, auto next) {
        res->res.set("Access-Control-Allow-Origin", "*");
        next();
    };
    m_Expresscpp->Use(headerMiddleWare);
    auto router = m_Expresscpp->GetRouter("api");
    m_Expresscpp->Use("/api", router);

    router->RegisterPath("/user/:username", expresscpp::HttpMethod::Options, [=](auto req, auto res){
        res->res.set("Access-Control-Allow-Methods", "GET,POST");
        res->res.set("Access-Control-Allow-Headers", ALLOWED_HEADERS);
        res->Send();
    });
    router->RegisterPath("/auth", expresscpp::HttpMethod::Options, [=](auto req, auto res){
        res->res.set("Access-Control-Allow-Methods", "GET,POST");
        res->res.set("Access-Control-Allow-Headers", ALLOWED_HEADERS);
        res->Send();
    });
    router->Post("/auth", [=](auto req, auto res) {
        std::cout << "URL: " << req->getUrl() << '\n';
        json response;
        for (auto [key, value] : req->getHeaders()){
            std::cout << key << " : " << value << '\n';
        }
        try{
            json body = json::parse(req->getBody());
            std::string username = body["username"].get<std::string>();
            std::string password = body["password"].get<std::string>();
            std::string token = authenticateUser(username, password);
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
        res->Json(response.dump());
    });
    router->Get("/user/:username", [](auto req, auto res) {
        json response;
        std::string username = req->GetParams().at("username");
        if (username == "BilboSwaggins"){
            response["name"] = username;
            response["id"] = 1;
            response["success"] = true;
            response["token"] = "BilboSwaggins:abc";
        }
        else{
            response["success"] = false;
        }
        res->Json(response.dump());
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

std::string NPCS_API_Server::authenticateUser(const std::string& username, const std::string& password){
    if (username == "BilboSwaggins" && password == "yourmom") return "BilboSwaggins:abc";
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