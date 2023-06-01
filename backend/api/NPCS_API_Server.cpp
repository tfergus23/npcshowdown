#include "api/NPCS_API_Server.hpp"
#define PORT 3000

NPCS_API_Server::NPCS_API_Server() : m_Expresscpp{std::make_shared<expresscpp::ExpressCpp>()}{
    m_Expresscpp->Get("/", [](auto /*req*/, auto res) { res->Send("hello world!"); });
}

int NPCS_API_Server::run(){
    m_Expresscpp->Listen(PORT,[=](auto /*ec*/) { std::cout << "Listening on port " << PORT << std::endl; }).Run();
    return 0;
}