#pragma once
#include "TrainerLevel.hpp"
#include "sim/battle/Battle.hpp"
#include "sim/battle/Move.hpp"
#include "tflib/static_vector.h"

class TrainerInfo{
public:
    std::string  name;
    TrainerLevel trainerLevel;

    TrainerInfo(const std::string& name, TrainerLevel level);
    const Move* pickMove(Pokemon* myPoke, Pokemon* enemyPoke, Battle* battle) const;
    int pickPokemon(Pokemon* currentlyActivePokemon, Pokemon* enemyPoke, Battle* battle) const;
    void getValidSwitches(Pokemon* currentlyActivePokemon, Battle* battle, tflib::static_vector<int,5>& outVec) const;
    int getValidSwitchesCount(Pokemon* currentlyActivePokemon, Battle* battle) const;
};