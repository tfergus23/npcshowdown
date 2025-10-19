#pragma once
#include "sim/battle/Battle.hpp"
#include "tfhttp/Server.hpp"
#include <deque>
#include "tflib/config.h"
#include <condition_variable>
#include "MariaDBConnection.hpp"

struct TournamentRequest{
    json requestJson;
    size_t id;
    size_t user = 0;
};

typedef std::chrono::time_point<std::chrono::high_resolution_clock> TimePoint;
struct FailedLoginState {
    uint32_t numFails = 0;
    TimePoint nextAllowed = std::chrono::high_resolution_clock::now();
    TimePoint nextReset = std::chrono::high_resolution_clock::now();

    bool canTryAgain();
    void increment();
    void reset();
private:
    std::mutex mut;

};

enum class ClientType : uint8_t{
    UNKNOWN = 0,
    SCRIPT,
    BROWSER
};

struct Credentials{
    std::string token;
    std::string error;
    ClientType type = ClientType::UNKNOWN;
};

namespace npcs{
    class Server{
    public:
        Server();
        ~Server();
        int run();
    private:
        // Load config file first
        tflib::ini_file config = tflib::ini_file("npcs_config.ini",{
            {"", "tournament_threads", "8"},
            {"", "db_user", "root"},
            {"", "db_password", ""},
            {"", "db_host", "localhost"},
            {"", "db_name", "npcs_test"},
            {"", "db_port", "3307"},
            {"", "max_user_sessions", "10"},
            {"", "max_trainers_per_user", "150"},
            {"", "website_url", "http://localhost:4200"},
            {"", "api_port", "3000"},
            {"", "serve_static", "0"},
            {"", "static_dir", "./static/"},
            {"", "keep_tournament_days", "7"},
            {"", "max_tournaments_per_day", "10000"},
            {"", "max_signups_per_day", "3"},
        }, false);

        // Config vars
        const int32_t max_tournament_threads = config.get_int("tournament_threads");
        const int32_t max_trainers_per_user = config.get_int("max_trainers_per_user");
        const std::string websiteURL = config.get("website_url");
        const std::string domain = getDomainFromURL();
        const int32_t port = config.get_int("api_port");
        const bool serveStatic = config.get_int("serve_static");
        const std::string staticDir = config.get("static_dir");
        const int32_t keepTournamentDays = config.get_int("keep_tournament_days");
        const int32_t maxTournamentsPerDay = config.get_int("max_tournaments_per_day");
        const int32_t maxSignUpsPerDay = config.get_int("max_signups_per_day");

        // Server state
        tfhttp::Server app = tfhttp::Server(tfhttp::Server::Options{
            .port = (uint16_t)port
        });
        std::deque<TournamentRequest>* queuedTournaments = nullptr;
        std::mutex* queuedTournamentMutexes = nullptr;
        int tournamentRequestThreadCounter = 0;
        std::mutex threadCounterMutex;
        std::unordered_map<size_t,int> idToThread;
        std::mutex idToThreadMutex;
        MariaDBConnection db = MariaDBConnection(config.get("db_user"), config.get("db_password"), config.get("db_host"), config.get("db_name"), config.get_int("max_user_sessions"), config.get("db_port"));
        std::unordered_map<std::string, uint32_t> ipSignUps;
        std::mutex ipSignUpsMutex;
        std::unordered_map<std::string, FailedLoginState> ipFailedLogins;
        std::mutex ipFailedLoginsMutex;

        //Canned data responses
        const std::string SPECIES_DATA_RESPONSE;
        const std::string ABILITY_DATA_RESPONSE;
        const std::string ITEM_DATA_RESPONSE;
        const std::string NATURE_DATA_RESPONSE;
        const std::string MOVE_DATA_RESPONSE;
        const std::string ALL_DATA_RESPONSE;

        std::string getDomainFromURL();
        void waitForTournaments(uint32_t threadNumber);
        size_t createTournamentRequest(const json& json, size_t user, const std::string& name, const std::string& ip);
        void startTournamentThreads();
    };
};