#pragma once
#include <string>
#include <array>
#include "battle/PokemonBlueprint.hpp"
#include "battle/Pokemon.hpp"
#include "battle/Move.hpp"
#include "battle/Battle.hpp"

enum TrainerLevel{FIRST_MOVE, USE_2_MOVES_THEN_SWITCH, SWITCHER, WILD, TRAINER, BOSS};

class Trainer{
public:
    std::string getTrainerClass();
    std::string getName();
    std::array<PokemonBlueprint, 6> getTeamBlueprint();
    TrainerLevel getTrainerLevel();

    Trainer(std::string trainerClass, std::string name, std::array<PokemonBlueprint, 6> teamBlueprint, TrainerLevel trainerLevel);
    Trainer(std::string filePath);

    std::string getFullName();

    const Move* pickMove(Pokemon* myPoke, Pokemon* enemyPoke, Battle* battle);

    int pickPokemon(Pokemon* currentlyActivePokemon, Pokemon* enemyPoke, Battle* battle);

    std::vector<int> getValidSwitches(Pokemon* currentlyActivePokemon, Battle* battle);

    std::string toJSON();
private:
    std::array<PokemonBlueprint, 6> m_TeamBlueprint;
    std::string m_TrainerClass;
    std::string m_Name;
    TrainerLevel m_TrainerLevel;
};