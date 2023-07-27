#include "api/NPCS_API_Server.hpp"
#include "nlohmann/json.hpp"
#include "sim/data/Species.hpp"


using json = nlohmann::json;

#define PORT 3000
#define WEBSITE_URL "http://localhost:4200"
#define ALLOWED_HEADERS "Authorization,Content-Type,X-Requested-With"
#define ALLOWED_METHODS "GET,POST,PUT,DELETE,OPTIONS"

NPCS_API_Server::NPCS_API_Server() : m_Expresscpp{std::make_shared<expresscpp::ExpressCpp>()}, m_SpeciesData{getSpeciesData()}{
    expresscpp::Console::setLogLevel(expresscpp::LogLevel::kDebug);
    auto headerMiddleWare = [&](auto req, auto res, auto next) {
        res->res.set("Access-Control-Allow-Origin", "*");
        next();
    };
    auto authorizedMiddleWare = [&](auto req, auto res, auto next) {
        std::string authorization = req->getHeader("Authorization");
        std::string tokenName = usernameFromToken(authorization);
        std::string requestName = req->GetParams().at("username");
        if (tokenName == requestName){
            next();
        }
        else{
            json response;
            response["success"] = false;
            response["message"] = "Unauthorized";
            res->SetStatus(401);
            res->Send(response.dump());
        }
    };
    m_Expresscpp->Use(headerMiddleWare);
    auto router = m_Expresscpp->GetRouter("noauth_api");
    auto authRouter = m_Expresscpp->GetRouter("auth_api");
    m_Expresscpp->Use("/api", router);
    router->Use("/user/:username", authRouter);
    authRouter->Use(authorizedMiddleWare);

    addPreflightHandler(router, "/auth");
    router->Post("/auth", [=](auto req, auto res) {
        json response;
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

    addPreflightHandler(router, "/user/:username");
    authRouter->Get("/", [](auto req, auto res) {
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
    addPreflightHandler(router, "/user/:username/logout");
    authRouter->Put("/logout", [](auto req, auto res) {
        res->Send();
        return;
        json response;
        //TODO: Set token to null in database
        response["success"] = true;
        res->Json(response.dump());
    });
    /*
    router->Get("/sim/tournament/:id", [](auto req, auto res) {res->Send("WIP!");});
    router->Get("/sim/battle/:id", [](auto req, auto res) {res->Send("WIP!");});
    router->Post("/sim/tournament", [](auto req, auto res) {res->Send("WIP!");});
    router->Post("/sim/battle", [](auto req, auto res) {res->Send("WIP!");});
    */
   m_Expresscpp->Stack();
}

int NPCS_API_Server::run(){
    m_Expresscpp->Listen(PORT,[=](auto /*ec*/) { std::cout << "Listening on port " << PORT << "\n\n"; }).Run();
    return 0;
}

std::string NPCS_API_Server::authenticateUser(const std::string& username, const std::string& password){
    if (username == "BilboSwaggins" && password == "yourmom") return "BilboSwaggins:abc";
    return "";
}
//Expresscpp doesn't support wildcards apparently so we need to manually add a handler for every preflight request...
void NPCS_API_Server::addPreflightHandler(expresscpp::RouterPtr router, const std::string& path){
    router->RegisterPath(path, expresscpp::HttpMethod::Options, [=](auto req, auto res){
        res->res.set("Access-Control-Allow-Methods", ALLOWED_METHODS);
        res->res.set("Access-Control-Allow-Headers", ALLOWED_HEADERS);
        res->SetStatus(200);
        res->Send();
    });
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