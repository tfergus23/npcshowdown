#include "nlohmann/json.hpp"
#include "sim/battle/Battle.hpp"
#include "sim/tournament/BattleResult.hpp"
#include "sim/tournament/Tournament.hpp"
#include "conncpp.hpp"
#include "api_utils.hpp"

using json = nlohmann::json;

struct TournamentResults{
    std::vector<TrainerStats> trainerStats;
    std::vector<size_t> trainers;
    bool ready = false;
};

class MariaDBConnection{
public:
    json getTrainer(size_t id);
    BattleResult getBattle(size_t id);
    TournamentResults getTournament(size_t id);
    std::vector<size_t> getUserTrainers(const std::string& username);
    size_t createEmptyTournament();
    size_t saveTrainer(const json& json);
    size_t saveBattle(const Trainer& trainer1, const Trainer& trainer2, size_t seed);
    size_t saveBattle(const BattleResult result); //TODO: Why is this not a reference?
    void saveTournament(const Tournament& tournament, size_t id);
    bool isTokenValid(const std::string& username, const std::string& token);
    std::string createUserSession(const std::string& username, const std::string& password, std::string& outToken);
    void deleteUserSession(const std::string& username, const std::string & token);
    void deleteOldUserSessions(size_t userID);
    size_t userIdFromName(const std::string& username);
    void updateTokenLastUsed(const std::string& username, const std::string& token);

    MariaDBConnection(const std::string& username, const std::string& password, const std::string& host, const std::string& database, int maxUserSessions);
private:
    std::unique_ptr<sql::Connection> conn;
    const int maxUserSessions = 1;
};
