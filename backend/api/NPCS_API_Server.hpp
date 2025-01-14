#pragma once
#include "sim/battle/Battle.hpp"
#include "tfhttp/HTTP_Server.hpp"
#include <deque>
#include "tflib/config.h"
#include <condition_variable>
#include "MariaDBConnection.hpp"

struct TournamentRequest{
    json requestJson;
    size_t id;
};

class NPCS_API_Server{
public:
    NPCS_API_Server();
    ~NPCS_API_Server();
    int run();
private:
    tfhttp::HTTP_Server app;
    tflib::ini_file config = tflib::ini_file("npcs_config.ini", false);
    int max_tournament_threads = 0;
    std::deque<TournamentRequest>* queuedTournaments = nullptr;
    std::mutex* queuedTournamentMutexes = nullptr;
    int tournamentRequestThreadCounter = 0;
    std::mutex threadCounterMutex;
    std::unordered_map<size_t,int> idToThread;
    std::mutex idToThreadMutex;
    MariaDBConnection db = MariaDBConnection(config.get("db_user"), config.get("db_password"), config.get("db_host"), config.get("db_name"), getIntFromConfig(config, "max_user_sessions"));

    std::string getToken(const std::string& username, const std::string& password);
    void waitForTournaments(uint32_t threadNumber);
    size_t createTournamentRequest(const json& json);
    void startTournamentThreads();
    int findTournamentPositionInQueue(size_t tournamentID);
};

std::string createAllDataResponse();