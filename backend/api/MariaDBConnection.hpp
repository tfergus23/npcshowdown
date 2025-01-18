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

struct User{
    size_t id;
    std::string name;
    std::string accountCreated;
    std::string lastPasswordChange;
    std::string email;
};

class MariaDBConnection{
public:
    Trainer getTrainer(size_t id);
    BattleResult getBattle(size_t id);
    TournamentResults getTournament(size_t id);
    std::vector<size_t> getUserTrainers(const std::string& username);
    size_t createEmptyTournament(size_t user);
    size_t saveTrainer(const Trainer& trainer, size_t user, size_t tournament);
    size_t saveBattle(const BattleResult& result, size_t tournament);
    void saveTournament(const Tournament& tournament, size_t id);
    bool isTokenValid(const std::string& username, const std::string& token);
    std::string createUserSession(const std::string& username, const std::string& password, std::string& outToken);
    void deleteUserSession(const std::string& username, const std::string & token);
    void updateTokenLastUsed(const std::string& username, const std::string& token);
    size_t userIdFromName(const std::string& username);
    std::optional<User> getUserData(const std::string& username);

    MariaDBConnection(const std::string& username, const std::string& password, const std::string& host, const std::string& database, int maxUserSessions);
private:
    std::unique_ptr<sql::Connection> conn;
    const int maxUserSessions = 1;
    void saveTeam(const std::vector<PokemonBlueprint>& team, size_t trainer);
    size_t executeInsertAndGetID(sql::PreparedStatement* stmnt);
    void executeInsert(sql::PreparedStatement* stmnt);
    void deleteOldUserSessions(size_t userID);
};
