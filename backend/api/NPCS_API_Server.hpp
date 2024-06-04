#pragma once
#include "sim/battle/Battle.hpp"
#include "tfhttp/HTTP_Server.hpp"
#include <queue>
#include "tflib/config.h"
#include <condition_variable>

struct TournamentRequest{
    json requestJson;
    size_t id;
};

const int MAX_TOURNAMENT_THREADS = 4;

class NPCS_API_Server{
public:
    NPCS_API_Server();
    int run();
private:
    tfhttp::HTTP_Server app;
    //tflib::ini_file config = tflib::ini_file("npcs_config.ini", false);
    //void addPreflightHandler(expresscpp::RouterPtr router, const std::string& path);
    std::string getToken(const std::string& username, const std::string& password);
    bool isTokenValid(const std::string& username, const std::string& token);
    std::array<std::queue<TournamentRequest>, MAX_TOURNAMENT_THREADS> queuedTournaments;
    std::array<std::mutex, MAX_TOURNAMENT_THREADS> queuedTournamentMutexes;
    void waitForTournaments(uint32_t threadNumber);
    size_t createTournamentRequest(const json& json);
    int tournamentRequestThreadCounter = 0;
    void startTournamentThreads();
};

std::string createAllDataResponse();