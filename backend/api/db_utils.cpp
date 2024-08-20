#include "db_utils.hpp"

// In-memory database for testing
static std::vector<json> savedTrainers;
static std::vector<BattleResult> savedBattles;
static std::vector<TournamentResults> savedTournaments;

static std::mutex saveTrainerMutex;
static std::mutex saveBattleMutex;
static std::mutex saveTournamentMutex;

json db::getTrainer(size_t id){
    saveTrainerMutex.lock();
    auto result = savedTrainers.at(id);
    saveTrainerMutex.unlock();
    return result;
}
BattleResult db::getBattle(size_t id){
    saveBattleMutex.lock();
    auto result = savedBattles.at(id);
    saveBattleMutex.unlock();
    return result;
}
TournamentResults db::getTournament(size_t id){
    saveTournamentMutex.lock();
    auto result = savedTournaments.at(id);
    saveTournamentMutex.unlock();
    return result;
}

size_t db::createEmptyTournament(){
    saveTournamentMutex.lock();
    savedTournaments.emplace_back();
    size_t id = savedTournaments.size() - 1;
    saveTournamentMutex.unlock();
    return id;
}

size_t db::saveTrainer(const json& json){
    saveTrainerMutex.lock();
    savedTrainers.push_back(json);
    size_t id = savedTrainers.size() - 1;
    saveTrainerMutex.unlock();
    return id;
}

size_t db::saveBattle(const Trainer& trainer1, const Trainer& trainer2, size_t seed){
    size_t t1ID = db::saveTrainer(trainer1.toJSON());
    size_t t2ID = db::saveTrainer(trainer2.toJSON());
    saveBattleMutex.lock();
    savedBattles.push_back({t1ID, t2ID, seed, 0}); // The 'winner' field isn't used in this case
    size_t id = savedBattles.size() - 1;
    saveBattleMutex.unlock();
    return id;
}

size_t db::saveBattle(const BattleResult result){
    saveBattleMutex.lock();
    savedBattles.push_back(result);
    size_t id = savedBattles.size() - 1;
    saveBattleMutex.unlock();
    return id;
}

void db::saveTournament(const Tournament& tournament, size_t id){
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
    for (auto stat : tournament.trainerStats){
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