#include "MariaDBConnection.hpp"
#include <iostream>
#include <mutex>
#include "api_utils.hpp"

// In-memory database for testing
static std::vector<json> savedTrainers;
static std::vector<BattleResult> savedBattles;
static std::vector<TournamentResults> savedTournaments;

static std::mutex saveTrainerMutex;
static std::mutex saveBattleMutex;
static std::mutex saveTournamentMutex;

MariaDBConnection::MariaDBConnection(const std::string& username, const std::string& password, const std::string& host, const std::string& database){
    sql::Driver* driver= sql::mariadb::get_driver_instance();
    sql::SQLString url("jdbc:mariadb://" + host + ":3306/" + database);
    sql::Properties properties({{"user", username}, {"password", password}});
    conn = std::unique_ptr<sql::Connection>(driver->connect(url, properties));
}

json MariaDBConnection::getTrainer(size_t id){
    std::unique_lock lk(saveTrainerMutex);
    auto result = savedTrainers.at(id);
    return result;
}
BattleResult MariaDBConnection::getBattle(size_t id){
    std::unique_lock lk(saveBattleMutex);
    auto result = savedBattles.at(id);
    return result;
}
TournamentResults MariaDBConnection::getTournament(size_t id){
    std::unique_lock lk(saveTournamentMutex);
    auto result = savedTournaments.at(id);
    return result;
}

size_t MariaDBConnection::createEmptyTournament(){
    std::unique_lock lk(saveTournamentMutex);
    savedTournaments.emplace_back();
    size_t id = savedTournaments.size() - 1;
    return id;
}

size_t MariaDBConnection::saveTrainer(const json& json){
    std::unique_lock lk(saveTrainerMutex);
    savedTrainers.push_back(json);
    size_t id = savedTrainers.size() - 1;
    return id;
}

size_t MariaDBConnection::saveBattle(const Trainer& trainer1, const Trainer& trainer2, size_t seed){
    size_t t1ID = saveTrainer(trainer1.toJSON());
    size_t t2ID = saveTrainer(trainer2.toJSON());
    std::unique_lock lk(saveBattleMutex);
    savedBattles.push_back({t1ID, t2ID, seed, 0}); // The 'winner' field isn't used in this case
    size_t id = savedBattles.size() - 1;
    return id;
}

size_t MariaDBConnection::saveBattle(const BattleResult result){
    std::unique_lock lk(saveBattleMutex);
    savedBattles.push_back(result);
    size_t id = savedBattles.size() - 1;
    return id;
}

void MariaDBConnection::saveTournament(const Tournament& tournament, size_t id){
    std::unique_lock lk(saveTournamentMutex);
    TournamentResults result = savedTournaments.at(id);
    std::vector<size_t> trainers;
    for (auto& trainer : tournament.trainers){
        trainers.push_back(saveTrainer(trainer.toJSON()));
    }
    result.trainers = trainers;

    std::vector<TrainerStats> stats;
    std::unordered_map<size_t,size_t> addedBattles;
    for (auto stat : tournament.trainerStats){ //TODO: Why isn't this a reference?
        if (!addedBattles.contains(stat.bestWin) && stat.bestWin >= 0){
            BattleResult bestWin = tournament.results[stat.bestWin];
            bestWin.trainer1 = trainers[bestWin.trainer1];
            bestWin.trainer2 = trainers[bestWin.trainer2];
            bestWin.winner = trainers[bestWin.winner];
            addedBattles[stat.bestWin] = saveBattle(bestWin);
        }
        stat.bestWin = addedBattles[stat.bestWin];
        stats.push_back(stat);
    }
    result.trainerStats = stats;

    result.ready = true;
    
    savedTournaments.at(id) = result;
}

std::vector<size_t> MariaDBConnection::getUserTrainers(const std::string& username){
    std::vector<size_t> result;
    size_t size = savedTrainers.size();
    result.reserve(size);
    for(size_t i = 0; i < size; i++){
        result.push_back(i);
    }
    return result;
}

std::string MariaDBConnection::createUserSession(const std::string& username, const std::string& password, std::string& outToken){
    std::unique_ptr<sql::PreparedStatement> passwordStatement(conn->prepareStatement("select id,password from user where name = ?"));
    passwordStatement->setString(1, username);
    std::unique_ptr<sql::ResultSet> results(passwordStatement->executeQuery());

    if (results->rowsCount() < 1){
        return "That user does not exist";
    }

    if (results->rowsCount() > 1){
        throw std::runtime_error("Somehow got two users in username query for: " + username);
    }

    std::string dbPassword;
    size_t userID = 0;
    while (results->next()){
        userID = results->getUInt64(1);
        dbPassword = results->getString(2);
    }

    std::string hashedPassword = sha256(password);

    if (hashedPassword != dbPassword){
        return "Invalid credentials";
    }

    //TODO: Code for too many sessions

    std::string newToken = generateUUID();
    std::unique_ptr<sql::PreparedStatement> insertTokenStatement(conn->prepareStatement("insert into user_session (user, token, dateGranted, lastUsed) values (?, ?, NOW(),NOW())"));
    insertTokenStatement->setUInt64(1, userID);
    insertTokenStatement->setString(2, newToken);
    delete insertTokenStatement->executeQuery();

    outToken = newToken;
    return "";
}

bool MariaDBConnection::checkCredentials(const std::string& username, const std::string& password){
    std::unique_ptr<sql::PreparedStatement> passwordStatement(conn->prepareStatement("select password from user where name = ?"));
    passwordStatement->setString(1, username);
    std::unique_ptr<sql::ResultSet> results(passwordStatement->executeQuery());

    if (results->rowsCount() < 1){
        return false;
    }

    if (results->rowsCount() > 1){
        throw std::runtime_error("Somehow got two users in username query for: " + username);
    }

    std::string hashedProvidedPassword = sha256(password);
    std::string dbPassword;
    while (results->next()){
        dbPassword = results->getString(1);
    }
    return hashedProvidedPassword == dbPassword;
}