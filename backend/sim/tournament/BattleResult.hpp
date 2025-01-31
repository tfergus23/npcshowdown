#pragma once
#include "nlohmann/json.hpp"

using json = nlohmann::json;

struct BattleResult{
    int trainer1 = -1;
    int trainer2 = -1;
    size_t seed = 0;
    int winner = -1;

    json toJSON() const;
};