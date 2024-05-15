#pragma once
#include "TournamentTrainer.hpp"
#include "BattleResult.hpp"
#include <random>

const int MIN_TOURNAMENT_TRAINERS = 2;
const int MAX_TOURNAMENT_BATTLES = 20000;

float calculateTotalBattles(int entrants, int rounds);

class Tournament{
public:
    std::vector<TournamentTrainer> trainers;
    std::vector<BattleResult> results;
    int rounds;
    float totalBattles;
    const size_t seed;

    Tournament(std::vector<Trainer>& trainers, int rounds, size_t seed);
    void run();
    std::vector<Trainer>& m_Trainers;
private:
    std::default_random_engine m_Generator;

    int randInt(int min, int max);
    void setBiggestUpsets();
};