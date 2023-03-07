#include "battle/Trainer.hpp"
#include <iostream>
#include <assert.h>
#include "data/Moves.hpp"



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
        if (myPoke->currentMoves[i] != nullptr && myPoke->currentPP[i] > 0 && i != myPoke->disabledIndex){
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
        if (getValidSwitches(myPoke, battle).size() > 0){
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
            if (getValidSwitches(myPoke, battle).size() > 0){
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
    auto validSlots = getValidSwitches(currentlyActivePokemon, battle);
    assert(validSlots.size() > 0);
    return validSlots[battle->randInt(0, validSlots.size())];
}

std::vector<int> Trainer::getValidSwitches(Pokemon* currentlyActivePokemon, Battle* battle){
    auto& myTeam = this == battle->getPlayer1() ? battle->player1Team : battle->player2Team;
    std::vector<int> validSlots;
    if (currentlyActivePokemon->isTrapped && !currentlyActivePokemon->isDead) return validSlots;
    for (int i = 0; i < myTeam.size(); i++){
        if (!myTeam[i].empty && !myTeam[i].isDead && currentlyActivePokemon != &(myTeam[i])){
            validSlots.push_back(i);
        }
    }
    return validSlots;
}

std::string Trainer::toJSON(){
    //TODO
    return "";
}