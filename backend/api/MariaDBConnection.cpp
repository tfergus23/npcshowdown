#include "MariaDBConnection.hpp"
#include <iostream>

// In-memory database for testing
static std::vector<json> savedTrainers;
static std::vector<BattleResult> savedBattles;
static std::vector<TournamentResults> savedTournaments;

static std::mutex saveTrainerMutex;
static std::mutex saveBattleMutex;
static std::mutex saveTournamentMutex;

MariaDBConneciton::MariaDBConneciton(const std::string& username, const std::string& password, const std::string& host, const std::string& database){
    sql::Driver* driver= sql::mariadb::get_driver_instance();
    sql::SQLString url("jdbc:mariadb://" + host + ":3306/" + database);
    sql::Properties properties({{"user", username}, {"password", password}});
    conn = std::unique_ptr<sql::Connection>(driver->connect(url, properties));
}

json MariaDBConneciton::getTrainer(size_t id){
    saveTrainerMutex.lock();
    auto result = savedTrainers.at(id);
    saveTrainerMutex.unlock();
    return result;
}
BattleResult MariaDBConneciton::getBattle(size_t id){
    saveBattleMutex.lock();
    auto result = savedBattles.at(id);
    saveBattleMutex.unlock();
    return result;
}
TournamentResults MariaDBConneciton::getTournament(size_t id){
    saveTournamentMutex.lock();
    auto result = savedTournaments.at(id);
    saveTournamentMutex.unlock();
    return result;
}

size_t MariaDBConneciton::createEmptyTournament(){
    saveTournamentMutex.lock();
    savedTournaments.emplace_back();
    size_t id = savedTournaments.size() - 1;
    saveTournamentMutex.unlock();
    return id;
}

size_t MariaDBConneciton::saveTrainer(const json& json){
    saveTrainerMutex.lock();
    savedTrainers.push_back(json);
    size_t id = savedTrainers.size() - 1;
    saveTrainerMutex.unlock();
    return id;
}

size_t MariaDBConneciton::saveBattle(const Trainer& trainer1, const Trainer& trainer2, size_t seed){
    size_t t1ID = saveTrainer(trainer1.toJSON());
    size_t t2ID = saveTrainer(trainer2.toJSON());
    saveBattleMutex.lock();
    savedBattles.push_back({t1ID, t2ID, seed, 0}); // The 'winner' field isn't used in this case
    size_t id = savedBattles.size() - 1;
    saveBattleMutex.unlock();
    return id;
}

size_t MariaDBConneciton::saveBattle(const BattleResult result){
    saveBattleMutex.lock();
    savedBattles.push_back(result);
    size_t id = savedBattles.size() - 1;
    saveBattleMutex.unlock();
    return id;
}

void MariaDBConneciton::saveTournament(const Tournament& tournament, size_t id){
    saveTournamentMutex.lock();
    TournamentResults result = savedTournaments.at(id);
    saveTournamentMutex.unlock();
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
    
    saveTournamentMutex.lock();
    savedTournaments.at(id) = result;
    saveTournamentMutex.unlock();
}

std::vector<size_t> MariaDBConneciton::getUserTrainers(const std::string& username){
    std::vector<size_t> result;
    size_t size = savedTrainers.size();
    result.reserve(size);
    for(size_t i = 0; i < size; i++){
        result.push_back(i);
    }
    return result;
}