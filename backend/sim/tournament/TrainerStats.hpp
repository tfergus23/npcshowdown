#pragma once
#include "sim/battle/Battle.hpp"
#include "limits.h"

class TrainerStats{
public:
    int trainerIndex = -1;
    int elo = 1000;
    int wins = 0;
    int losses = 0;
    int opponentRatingsTotal = 0;
    //Index into the results list
    int bestWin = -1;
    int bestWinEloDiff = INT_MIN;

    void calculateElo();
    json toJSON() const;
};