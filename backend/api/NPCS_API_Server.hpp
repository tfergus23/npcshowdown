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
    size_t user = 0;
};

class NPCS_API_Server{
public:
    NPCS_API_Server();
    ~NPCS_API_Server();
    int run();
private:
    tflib::ini_file config = tflib::ini_file("npcs_config.ini",{
        {"", "tournament_threads", "8"},
        {"", "db_user", "root"},
        {"", "db_password", "password"},
        {"", "db_host", "localhost"},
        {"", "db_name", "npcs_test"},
        {"", "max_user_sessions", "10"},
        {"", "max_trainers_per_user", "150"},
        {"", "website_url", "http://localhost:4200"},
        {"", "api_port", "3000"},
        {"", "serve_static", "0"},
        {"", "static_dir", "./static/"},
        {"", "keep_tournament_days", "7"},
    }, false);

    //Config vars
    const int max_tournament_threads = getIntFromConfig(config, "tournament_threads");
    const int max_trainers_per_user = getIntFromConfig(config, "max_trainers_per_user");
    const std::string websiteURL = config.get("website_url");
    const std::string domain = getDomainFromURL();
    const int port = getIntFromConfig(config, "api_port");
    const bool serveStatic = getIntFromConfig(config, "serve_static");
    const std::string staticDir = config.get("static_dir");
    const int keepTournamentDays = getIntFromConfig(config, "keep_tournament_days");

    tfhttp::HTTP_Server app = tfhttp::HTTP_Server(tfhttp::HTTP_Server::Options{
        .port = (uint16_t)port
    });
    std::deque<TournamentRequest>* queuedTournaments = nullptr;
    std::mutex* queuedTournamentMutexes = nullptr;
    int tournamentRequestThreadCounter = 0;
    std::mutex threadCounterMutex;
    std::unordered_map<size_t,int> idToThread;
    std::mutex idToThreadMutex;
    MariaDBConnection db = MariaDBConnection(config.get("db_user"), config.get("db_password"), config.get("db_host"), config.get("db_name"), getIntFromConfig(config, "max_user_sessions"));

    //Canned data responses
    const std::string SPECIES_DATA_RESPONSE;
    const std::string ABILITY_DATA_RESPONSE;
    const std::string ITEM_DATA_RESPONSE;
    const std::string NATURE_DATA_RESPONSE;
    const std::string MOVE_DATA_RESPONSE;
    const std::string ALL_DATA_RESPONSE;

    std::string getToken(const std::string& username, const std::string& password);
    std::string getDomainFromURL();
    void waitForTournaments(uint32_t threadNumber);
    size_t createTournamentRequest(const json& json, size_t user);
    void startTournamentThreads();
    int findTournamentPositionInQueue(size_t tournamentID, int threadNumber);
    void testTrainerSerialization();
};

std::string createAllDataResponse();
