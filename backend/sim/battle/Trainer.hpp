#pragma once
#include <vector>
#include <string>
#include "sim/battle/PokemonBlueprint.hpp"
#include "sim/battle/Move.hpp"
#include "sim/battle/Pokemon.hpp"
#include "sim/battle/Battle.hpp"
#include "tflib/static_vector.h"

enum TrainerLevel{FIRST_MOVE, USE_2_MOVES_THEN_SWITCH, SWITCHER, WILD, TRAINER, BOSS};


class Trainer{
public:
/*
    std::string getTrainerClass();
    std::string getName();
    std::array<PokemonBlueprint, 6> getTeamBlueprint();
    TrainerLevel getTrainerLevel();
*/
    const std::vector<PokemonBlueprint> teamBlueprint;
    const std::string trainerClass;
    const std::string name;
    const TrainerLevel trainerLevel;


    Trainer(std::string_view trainerClass, std::string_view name, const std::vector<PokemonBlueprint>& teamBlueprint, TrainerLevel trainerLevel);
    Trainer(json json);

    std::string getFullName() const;

    const Move* pickMove(Pokemon* myPoke, Pokemon* enemyPoke, Battle* battle);

    int pickPokemon(Pokemon* currentlyActivePokemon, Pokemon* enemyPoke, Battle* battle);

    void getValidSwitches(Pokemon* currentlyActivePokemon, Battle* battle, tflib::static_vector<int,5>& outVec);
    int getValidSwitchesCount(Pokemon* currentlyActivePokemon, Battle* battle);

    json toJSON() const;
    bool equals(const Trainer& that) const;

    static TrainerLevel levelFromString(const std::string& string);
private:
};