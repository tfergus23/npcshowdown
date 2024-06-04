#include "sim/battle/Battle.hpp"
#include <string.h>
#include "sim/data/Moves.hpp"
#include <unordered_set>
#include "sim/utils/move_functions.hpp"

TrainerInfo::TrainerInfo(const std::string& name, TrainerLevel level) : trainerLevel{level}, name{name}
{
}
void findMostDamagingMove(Pokemon* myPoke, Pokemon* enemyPoke, const std::unordered_set<const Move*>& myMoves, const Move*& outMove, int& outDamage){
    for (auto move : myMoves){
        if (move->damageCategory == DamageCategory::STATUS){
            return;
        }
        MoveUse moveUse(move, myPoke, enemyPoke, myPoke->battle);
        int avgDamage = calculateDirectDamage(&moveUse, true).damage;
        if (avgDamage > outDamage){
            outMove = move;
            outDamage = avgDamage;
        }
    }
}

const Move* pickSmartMove(Pokemon* myPoke, Pokemon* enemyPoke,  Battle* battle){
    std::unordered_set<const Move*> validMoves;
    bool isPlayer1 = myPoke == battle->player1ActivePokemon;
    auto& myTeam = isPlayer1 ?  battle->player1Team : battle->player2Team;
    int& switchCounter = isPlayer1 ? battle->player1SwitchCounter : battle->player2SwitchCounter;
    for (int i = 0; i < myPoke->currentMoves.size(); i++){
        const Move* move = myPoke->currentMoves[i];
        if (move != MOVE_NONE && myPoke->currentPP[i] > 0 && !myPoke->isMoveDisabled(move)){
            validMoves.insert(move);
        }
    }
    const Move* mostDamagingMove = nullptr;
    int mostDamage = 0;
    float mostDamagePercent = (float) mostDamage / enemyPoke->getStat(Stat::HP);
    findMostDamagingMove(myPoke, enemyPoke, validMoves, mostDamagingMove, mostDamage);

    float myHealthPercent = ((float) myPoke->currentHealth / myPoke->getStat(Stat::HP)) * 100;
    float enemyHealthPercent = ((float) enemyPoke->currentHealth / enemyPoke->getStat(Stat::HP)) * 100;

    bool imFaster = myPoke->getStat(Stat::SPEED) > enemyPoke->getStat(Stat::SPEED);

    if (mostDamagingMove != nullptr && mostDamage >= enemyPoke->currentHealth){
        return mostDamagingMove;
    }
    else if (mostDamagingMove != nullptr){
        return mostDamagingMove;
    }
    else{
        int i = 0;
        int rand = battle->randInt(0,validMoves.size());
        for (auto move : validMoves){
            if (i == rand){
                return move;
            }
            i++;
        }
        battle->assertTrue(false, "pickSmartMove didn't pick a random move properly");
        return nullptr;
    }
}

const Move* TrainerInfo::pickMove(Pokemon* myPoke, Pokemon* enemyPoke, Battle* battle) const{
    tflib::static_vector<const Move*, 4> validMoves;
    bool isPlayer1 = this == battle->getPlayer1();
    auto& myTeam = isPlayer1 ?  battle->player1Team : battle->player2Team;
    int& switchCounter = isPlayer1 ? battle->player1SwitchCounter : battle->player2SwitchCounter;
    for (int i = 0; i < myPoke->currentMoves.size(); i++){
        const Move* move = myPoke->currentMoves[i];
        if (move != MOVE_NONE && myPoke->currentPP[i] > 0 && !myPoke->isMoveDisabled(move)){
            validMoves.push_back(move);
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
            switchCounter++;
            if (validMoves.size() <= 0){
                return &MOVE_STRUGGLE;
            }
            return validMoves[battle->randInt(0, validMoves.size())];
        }
    case TRAINER:{
        if (validMoves.size() <= 0){
            return &MOVE_STRUGGLE;
        }
        int rand = battle->randInt(0,2);
        switch (rand)
        {
        case 0:
            return validMoves[battle->randInt(0, validMoves.size())];
        
        case 1:
            return pickSmartMove(myPoke, enemyPoke, battle);
        }}
    case BOSS:
        if (validMoves.size() <= 0){
            return &MOVE_STRUGGLE;
        }
        return pickSmartMove(myPoke, enemyPoke, battle);
    default:
        battle->assertTrue(false, "Unimplemented trainer level: " + stringFromTrainerLevel(trainerLevel));
    }
    return nullptr;
}

int TrainerInfo::pickPokemon(Pokemon* currentlyActivePokemon, Pokemon* enemyPoke, Battle* battle) const{
    auto& myTeam = this == battle->getPlayer1() ? battle->player1Team : battle->player2Team;
    tflib::static_vector<int,5> validSlots;
    getValidSwitches(currentlyActivePokemon, battle, validSlots);
    battle->assertTrue(validSlots.size() > 0, "pickPokemon called without any valid pokemon to switch to.");
    return validSlots[battle->randInt(0, validSlots.size())];
}

void TrainerInfo::getValidSwitches(Pokemon* currentlyActivePokemon, Battle* battle, tflib::static_vector<int,5>& outVec) const{
    auto& myTeam = this == battle->getPlayer1() ? battle->player1Team : battle->player2Team;
    if (currentlyActivePokemon->isTrapped && !currentlyActivePokemon->isDead) return;
    for (int i = 0; i < myTeam.size(); i++){
        if (!myTeam[i].empty && !myTeam[i].isDead && currentlyActivePokemon != &(myTeam[i])){
            outVec.push_back(i);
        }
    }
}

int TrainerInfo::getValidSwitchesCount(Pokemon* currentlyActivePokemon, Battle* battle) const{
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

