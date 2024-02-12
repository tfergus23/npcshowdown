#pragma once
#include "TournamentTrainer.hpp"
#include "BattleResult.hpp"
#include <random>

const int MIN_TOURNAMENT_TRAINERS = 2;
const int MAX_TOURNAMENT_BATTLES = 20000;

int calculateTotalBattles(int entrants, int rounds);

class Tournament{
public:
    std::vector<TournamentTrainer> trainers;
    std::vector<BattleResult> results;
    int rounds;
    int totalBattles;
    const int seed;

    Tournament(std::vector<Trainer>& trainers, int rounds, int seed);
    std::vector<TournamentTrainer> run();
private:
    std::default_random_engine m_Generator;

    int randInt(int min, int max);
    void setBiggestUpsets();
};