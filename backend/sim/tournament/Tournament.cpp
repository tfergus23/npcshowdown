#include "Tournament.hpp"

float calculateTotalBattles(int entrants, int rounds){
    //Using floats to avoid overflow issues
    float battles = (float) entrants * ((float) entrants - 1) / 2;
    return battles * rounds;
}

Tournament::Tournament(std::vector<Trainer>& trainers, int rounds, size_t seed) : rounds{rounds}, totalBattles{calculateTotalBattles(trainers.size(), rounds)}, m_Generator{std::default_random_engine(seed)}, seed{seed}, trainers{trainers}{
    if (rounds < 1){
        throw std::runtime_error("Can't have a tournament with no rounds... unless you don't want me to do anything. Rounds requested: " + std::to_string(rounds));
    }
    if (totalBattles > MAX_TOURNAMENT_BATTLES){
        throw std::runtime_error("Cant have more than " + std::to_string(MAX_TOURNAMENT_BATTLES) + ", tried " + std::to_string(totalBattles));
    }
    if (trainers.size() < MIN_TOURNAMENT_TRAINERS){
        throw std::runtime_error("Need at least " + std::to_string(MIN_TOURNAMENT_TRAINERS) + " trainers for a tournament, " + std::to_string(trainers.size()) + " supplied.");
    }

    this->m_Results.reserve(totalBattles);

    int id = 0;
    for (auto& trainer : trainers){
        this->trainerStats.push_back(
            {
                .trainerIndex = id++
            }
        );
    }
}

int determineBattleWinner(Battle& battle, int trainer1, int trainer2){
    if (!battle.isBattleOver){
        throw std::runtime_error("Can't determine result if the battle was never ran.");
    }
    if (battle.isDraw){
        return -1; //TODO: Is this a problem?
    }
    else{
        return battle.winner == battle.getPlayer1() ? trainer1 : trainer2;
    }
}

void Tournament::run(){
    for(int i = 0; i < rounds; i++){
        for(int j = 0; j < trainers.size(); j++){
            for(int k = j + 1; k < trainers.size(); k++){
                int trainer1 = j;
                int trainer2 = k;
                assert(this->trainerStats[j].trainerIndex == j);
                assert(this->trainerStats[k].trainerIndex == k);
                Battle battle(trainers[trainer1], trainers[trainer2], randInt(0, INT_MAX));
                battle.doLogging = false;
                battle.simulate();
                if (battle.invalid){
                    errorBattles.push_back({j, k, battle.getSeed(), -1});
                    continue;
                }
                int winner = determineBattleWinner(battle, j, k);
                m_Results.push_back({j, k, battle.getSeed(), winner});
                if (!battle.isDraw){
                    int loserIndex = winner == trainer1 ? trainer2 : trainer1;
                    TrainerStats& winnerStats = trainerStats[winner];
                    TrainerStats& loserStats = trainerStats[loserIndex];
                    winnerStats.wins++;
                    loserStats.losses++;
                    winnerStats.opponentRatingsTotal += loserStats.elo;
                    loserStats.opponentRatingsTotal += winnerStats.elo;
                }
            }
        }
        for (auto& stats : trainerStats){
            stats.calculateElo();
        }
    }
    setBiggestUpsets();
}

int Tournament::randInt(int min, int max){
    std::uniform_int_distribution<int> distribution(min, max-1);
    int dice_roll = distribution(m_Generator);
    return dice_roll;
}

void Tournament::setBiggestUpsets(){
    for (int i = 0; i < m_Results.size(); i++){
        BattleResult& result = m_Results[i];

        if (result.winner == -1) continue;
        TrainerStats& winner = trainerStats[result.winner];
        TrainerStats& loser = trainerStats[result.winner == result.trainer1 ? result.trainer2 : result.trainer1];

        int winnerEloDiff = loser.elo - winner.elo;
        if (winnerEloDiff > winner.bestWinEloDiff){
            winner.bestWin = result;
            winner.bestWinEloDiff = winnerEloDiff;
        }
    }
}