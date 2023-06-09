#pragma once
#include "sim/battle/Battle.hpp"
#include "expresscpp/expresscpp.hpp"

class NPCS_API_Server{
public:
    NPCS_API_Server();
    int run();
private:
    const std::shared_ptr<expresscpp::ExpressCpp> m_Expresscpp;
    std::string authenticateUser(const std::string& username, const std::string& password);
    std::string getSpeciesData();
    std::string getAbilityData();
    std::string getItemData();
    std::string getMoveData();
    const std::string m_SpeciesData;
    const std::string m_AbilityData;
    const std::string m_ItemData;
    const std::string m_MoveData;
};