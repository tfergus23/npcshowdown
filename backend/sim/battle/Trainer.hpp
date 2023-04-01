#pragma once
#include <string>
#include <array>
#include "sim/battle/PokemonBlueprint.hpp"
#include "sim/battle/Pokemon.hpp"
#include "sim/battle/Move.hpp"
#include "sim/battle/Battle.hpp"

enum TrainerLevel{FIRST_MOVE, USE_2_MOVES_THEN_SWITCH, SWITCHER, WILD, TRAINER, BOSS};

class Trainer{
public:
/*
    std::string getTrainerClass();
    std::string getName();
    std::array<PokemonBlueprint, 6> getTeamBlueprint();
    TrainerLevel getTrainerLevel();
*/
    const std::array<PokemonBlueprint, 6> teamBlueprint;
    const std::string trainerClass;
    const std::string name;
    const TrainerLevel trainerLevel;


    Trainer(std::string trainerClass, std::string name, const std::array<PokemonBlueprint, 6>& teamBlueprint, TrainerLevel trainerLevel);

    std::string getFullName();

    const Move* pickMove(Pokemon* myPoke, Pokemon* enemyPoke, Battle* battle);

    int pickPokemon(Pokemon* currentlyActivePokemon, Pokemon* enemyPoke, Battle* battle);

    std::vector<int> getValidSwitches(Pokemon* currentlyActivePokemon, Battle* battle);

    std::string toJSON();
private:
};