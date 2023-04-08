#include "sim/battle/Trainer.hpp"
#include <iostream>
#include "sim/data/Moves.hpp"



Trainer::Trainer(std::string trainerClass, std::string name, const std::array<PokemonBlueprint, 6>& teamBlueprint, TrainerLevel trainerLevel) : 
teamBlueprint{teamBlueprint},
trainerClass{trainerClass},
name{name},
trainerLevel{trainerLevel}
{
}


std::string Trainer::getFullName(){
    if (trainerClass == ""){
        return name;
    }
    return trainerClass + " " + name;
}

const Move* Trainer::pickMove(Pokemon* myPoke, Pokemon* enemyPoke, Battle* battle){
    std::vector<const Move*> validMoves;
    bool isPlayer1 = this == battle->getPlayer1();
    auto& myTeam = isPlayer1 ?  battle->player1Team : battle->player2Team;
    int& switchCounter = isPlayer1 ? battle->player1SwitchCounter : battle->player2SwitchCounter;
    for (int i = 0; i < myPoke->currentMoves.size(); i++){
        if (myPoke->currentMoves[i] != MOVE_NONE && myPoke->currentPP[i] > 0 && i != myPoke->disabledIndex){
            validMoves.push_back(myPoke->currentMoves[i]);
        }
    }


    //TODO: The rest of these
    switch (trainerLevel)
    {
    case FIRST_MOVE:
        if (validMoves.size() <= 0){
            return &MOVE_STRUGGLE;
        }
        return validMoves[0];
    case SWITCHER:
        if (getValidSwitchesCount(myPoke, battle) > 0){
            return &MOVE_SWITCH;
        }
        if (validMoves.size() <= 0){
            return &MOVE_STRUGGLE;
        }
        return validMoves[battle->randInt(0, validMoves.size())];
    case WILD:
        if (validMoves.size() <= 0){
            return &MOVE_STRUGGLE;
        }
        return validMoves[battle->randInt(0, validMoves.size())];
    case USE_2_MOVES_THEN_SWITCH:
        if (switchCounter > 2){
            if (getValidSwitchesCount(myPoke, battle) > 0){
                switchCounter = 0;
                return &MOVE_SWITCH;
            }
            else{
                if (validMoves.size() <= 0){
                    return &MOVE_STRUGGLE;
                }
                return validMoves[battle->randInt(0, validMoves.size())];
            }
        }
        else{
            if (validMoves.size() <= 0){
                return &MOVE_STRUGGLE;
            }
            return validMoves[battle->randInt(0, validMoves.size())];
        }
    case TRAINER:
        std::cerr << "Unhandled trainer level.\n";
        throw 1;
    case BOSS:
        std::cerr << "Unhandled trainer level.\n";
        throw 1;
    default:
        std::cerr << "Unhandled trainer level.\n";
        throw 1;
    }
    return nullptr;
}

int Trainer::pickPokemon(Pokemon* currentlyActivePokemon, Pokemon* enemyPoke, Battle* battle){
    auto& myTeam = this == battle->getPlayer1() ? battle->player1Team : battle->player2Team;
    std::vector<int> validSlots;
    getValidSwitches(currentlyActivePokemon, battle, validSlots);
    battle->assert(validSlots.size() > 0, "pickPokemon called without any valid pokemon to switch to.");
    return validSlots[battle->randInt(0, validSlots.size())];
}

void Trainer::getValidSwitches(Pokemon* currentlyActivePokemon, Battle* battle, std::vector<int>& outVec){
    auto& myTeam = this == battle->getPlayer1() ? battle->player1Team : battle->player2Team;
    if (currentlyActivePokemon->isTrapped && !currentlyActivePokemon->isDead) return;
    for (int i = 0; i < myTeam.size(); i++){
        if (!myTeam[i].empty && !myTeam[i].isDead && currentlyActivePokemon != &(myTeam[i])){
            outVec.push_back(i);
        }
    }
}

int Trainer::getValidSwitchesCount(Pokemon* currentlyActivePokemon, Battle* battle){
    auto& myTeam = this == battle->getPlayer1() ? battle->player1Team : battle->player2Team;
    if (currentlyActivePokemon->isTrapped && !currentlyActivePokemon->isDead) return 0;
    int result = 0;
    for (int i = 0; i < myTeam.size(); i++){
        if (!myTeam[i].empty && !myTeam[i].isDead && currentlyActivePokemon != &(myTeam[i])){
            result++;
        }
    }
    return result;
}

std::string Trainer::toJSON(){
    //TODO
    return "";
}