#pragma once
#include "sim/battle/Battle.hpp"
#include "sim/tournament/BattleResult.hpp"
#include "limits.h"

class TrainerStats{
public:
    int trainerIndex = -1;
    int elo = 1000;
    int wins = 0;
    int losses = 0;
    int opponentRatingsTotal = 0;
    BattleResult bestWin;
    int bestWinEloDiff = INT_MIN;

    void calculateElo();
    json toJSON() const;
};