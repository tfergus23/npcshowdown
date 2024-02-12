#include "Tournament.hpp"

int calculateTotalBattles(int entrants, int rounds){
    int battles = 0;
    for (int i = 0; i < rounds; i++)
    {
        for (int j = 0; j < entrants; j++)
        {
            for (int k = j+1; k < entrants; k++)
            {
                battles++;
            }
        }
    }
    return battles;
}

Tournament::Tournament(std::vector<Trainer>& trainers, int rounds, int seed) : rounds{rounds}, totalBattles{calculateTotalBattles(trainers.size(), rounds)}, m_Generator{std::default_random_engine(seed)}, seed{seed}{
    if (totalBattles > MAX_TOURNAMENT_BATTLES){
        throw std::runtime_error("Cant have more than " + std::to_string(MAX_TOURNAMENT_BATTLES) + ", tried " + std::to_string(totalBattles));
    }
    if (trainers.size() < MIN_TOURNAMENT_TRAINERS){
        throw std::runtime_error("Need at least " + std::to_string(MIN_TOURNAMENT_TRAINERS) + " trainers for a tournament, " + std::to_string(trainers.size()) + " supplied.");
    }

    this->trainers.reserve(trainers.size());
    this->results.reserve(totalBattles);

    int id = 0;
    for (auto& trainer : trainers){
        this->trainers.push_back(
            {
                .trainer = &trainer
            }
        );
    }
}

BattleResult determineBattleResult(Battle& battle, int trainer1, int trainer2){
    if (!battle.isBattleOver){
        throw std::runtime_error("Can't determine result if the battle was never ran.");
    }

    BattleResult result;
    if (battle.isDraw){
        result.winner = -1;
    }
    else{
        result.winner = battle.winner == battle.getPlayer1() ? trainer1 : trainer2;
    }
    result.trainer1 = trainer1;
    result.trainer2 = trainer2;
    result.seed = battle.getSeed();
    return result;
}

std::vector<TournamentTrainer> Tournament::run(){
    for(int i = 0; i < rounds; i++){
        for(int j = 0; j < trainers.size(); j++){
            for(int k = j + 1; k < trainers.size(); k++){
                Battle battle(this->trainers[j].trainer, this->trainers[k].trainer, randInt(0, INT_MAX));
                battle.simulate();
                BattleResult result = determineBattleResult(battle, j, k);
                results.push_back(result);
                if (!battle.isDraw){
                    int loserIndex = result.winner == result.trainer1 ? result.trainer2 : result.trainer1;
                    TournamentTrainer& winner = trainers[result.winner];
                    TournamentTrainer& loser = trainers[loserIndex];
                    winner.wins++;
                    loser.losses++;
                    winner.opponentRatingsTotal += loser.elo;
                    loser.opponentRatingsTotal += winner.elo;
                    winner.calculateElo();
                    loser.calculateElo();
                }
            }
        }
    }
    setBiggestUpsets();
    return trainers;
}

int Tournament::randInt(int min, int max){
    std::uniform_int_distribution<int> distribution(min, max-1);
    int dice_roll = distribution(m_Generator);
    return dice_roll;
}

void Tournament::setBiggestUpsets(){
    for (int i = 0; i < results.size(); i++){
        BattleResult& result = results[i];

        if (result.winner == -1) continue;
        TournamentTrainer& winner = trainers[result.winner];
        TournamentTrainer& loser = trainers[result.winner == result.trainer1 ? result.trainer2 : result.trainer1];

        int winnerEloDiff = loser.elo - winner.elo;
        if (winnerEloDiff > winner.bestWinEloDiff){
            winner.bestWin = i;
            winner.bestWinEloDiff = winnerEloDiff;
        }
    }
}