#pragma once
#include "TrainerStats.hpp"
#include "BattleResult.hpp"
#include <random>

constexpr int MIN_TOURNAMENT_TRAINERS = 2;
constexpr int MAX_TOURNAMENT_BATTLES = 20000;

float calculateTotalBattles(int entrants, int rounds);

class Tournament{
public:
    std::vector<TrainerStats> trainerStats;
    std::vector<BattleResult> results;
    int rounds;
    float totalBattles;
    const size_t seed;

    Tournament(std::vector<Trainer>& trainers, int rounds, size_t seed);
    void run();
    std::vector<Trainer>& trainers;
private:
    std::default_random_engine m_Generator;

    int randInt(int min, int max);
    void setBiggestUpsets();
};