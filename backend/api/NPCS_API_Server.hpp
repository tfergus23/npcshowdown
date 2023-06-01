#pragma once
#include "sim/battle/Battle.hpp"
#include "expresscpp/expresscpp.hpp"

class NPCS_API_Server{
public:
    NPCS_API_Server();
    int run();
private:
    const std::shared_ptr<expresscpp::ExpressCpp> m_Expresscpp;
};