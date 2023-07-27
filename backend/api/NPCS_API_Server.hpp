#pragma once
#include "sim/battle/Battle.hpp"
#include "tfhttp/HTTP_Server.hpp"
using namespace tfhttp;

class NPCS_API_Server{
public:
    NPCS_API_Server();
    int run();
private:
    HTTP_Server app;
    std::string authenticateUser(const std::string& username, const std::string& password);
    std::string getSpeciesData();
    std::string getAbilityData();
    std::string getItemData();
    std::string getMoveData();
    //void addPreflightHandler(expresscpp::RouterPtr router, const std::string& path);
    bool isTokenValid(const std::string token);
    const std::string m_SpeciesData;
    const std::string m_AbilityData;
    const std::string m_ItemData;
    const std::string m_MoveData;
};

std::string usernameFromToken(const std::string& token);