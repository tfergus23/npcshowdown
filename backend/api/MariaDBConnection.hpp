#include "nlohmann/json.hpp"
#include "sim/battle/Battle.hpp"
#include "sim/tournament/BattleResult.hpp"
#include "sim/tournament/Tournament.hpp"
#include "conncpp.hpp"
#include "./Utils.hpp"
#include <optional>

using json = nlohmann::json;

class MariaDBConnection;
struct TournamentResults{
    std::vector<TrainerStats> trainerStats;
    std::vector<size_t> trainers;
    std::string user = "";
    std::string name = "";
    std::string dateRan = "";
    size_t id = 0;
    bool ready = false;

    json toJSON() const;
};

struct User{
    size_t id;
    std::string name;
    std::string accountCreated;
    std::string lastPasswordChange;
    bool isAdmin;
};

struct ErrorBattle{
    std::string request;
    std::string dateRan;
    size_t hash = 0;
};

class MariaDBConnection{
public:
    MariaDBConnection(const std::string& username, const std::string& password, const std::string& host, const std::string& database, int maxUserSessions, const std::string& port);

    std::optional<Trainer> getTrainer(size_t id);
    std::optional<TournamentResults> getTournament(size_t id);
    bool tournamentExists(size_t id);
    bool userTrainerExists(size_t id, size_t user);
    size_t createEmptyTournament(size_t user, const std::string& name, const std::string& ip);
    size_t saveTrainer(const Trainer& trainer, size_t user, size_t tournament, size_t existingID=0);
    void saveTournament(const Tournament& tournament, size_t id);
    bool isTokenValid(const std::string& username, const std::string& token);
    std::string createUserSession(const std::string& username, const std::string& password, std::string& outToken);
    bool deleteUserSession(const std::string& username, const std::string & token);
    void updateTokenLastUsed(const std::string& username, const std::string& token);
    size_t userIdFromName(const std::string& username);
    std::optional<User> getUserData(const std::string& username);
    std::vector<json> getUserTrainers(const std::string& username);
    std::vector<TournamentResults> getUserTournaments(const std::string& username);
    void saveTournamentToUser(size_t user, size_t tournament);
    void deleteSavedTrainer(size_t user, size_t trainer);
    void deleteSavedTournament(size_t user, size_t tournament);
    void updateSavedTrainer(size_t trainer, size_t user, const Trainer& trainerData);
    int userTrainerCount(size_t user);
    bool userHasTournamentSaved(size_t user, size_t tournament);
    bool tournamentTrainerExists(size_t trainer);
    bool isUserPasswordCorrect(const std::string& username, const std::string& password);
    void updateUserPassword(const std::string& username, const std::string& newPassword);
    size_t createUser(const std::string& username, const std::string& password);
    bool deleteUser(const std::string& username);
    int deleteOldTournaments(int maxAllowedDays);
    bool updateTournamentName(size_t tournament, const std::string& username, const std::string& newName);
    bool tournamentHasName(size_t tournament, const std::string& name);
    void saveErrorBattle(const std::string& battleJsonDump);
    size_t getTotalErrorBattles();
    std::vector<ErrorBattle> getErrorBattles(uint32_t page, uint32_t count);
    bool deleteErrorBattle(size_t hash);
    size_t getTournamentsFromIPToday(const std::string& ip);
private:
    std::unique_ptr<sql::Connection> conn;
    const int maxUserSessions = 1;

    void saveTeam(const std::vector<PokemonBlueprint>& team, size_t trainer);
    size_t executeInsertAndGetID(sql::PreparedStatement* stmnt);
    void executeInsert(sql::PreparedStatement* stmnt);
    void deleteOldUserSessions(size_t userID);
    std::optional<BattleResult> getBattle(size_t id);
    size_t saveBattle(int trainer1, int trainer2, size_t seed, size_t tournament);
};
