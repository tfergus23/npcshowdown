#pragma once
#include <vector>
#include <string>
#include "sim/battle/PokemonBlueprint.hpp"
#include "sim/battle/Move.hpp"
#include "sim/battle/Pokemon.hpp"
#include "sim/battle/Battle.hpp"
#include "tflib/static_vector.h"

class Trainer{
public:
/*
    std::string getTrainerClass();
    std::string getName();
    std::array<PokemonBlueprint, 6> getTeamBlueprint();
    TrainerLevel getTrainerLevel();
*/
    const std::vector<PokemonBlueprint> teamBlueprint;
    const TrainerInfo trainerInfo;


    Trainer(const std::string& name, const std::vector<PokemonBlueprint>& teamBlueprint, TrainerLevel trainerLevel);
    Trainer(json json);

    std::string getFullName() const;

    json toJSON() const;
    bool equals(const Trainer& that) const;
private:
};