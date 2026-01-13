#include "sim/battle/Battle.hpp"
#include <string.h>
#include "sim/data/Moves.hpp"
#include <unordered_set>
#include <set>
#include "sim/utils/MoveFunctions.hpp"

static const std::vector<const Move*> poisoningMoves = {
    &MOVE_TOXIC,
    &MOVE_POISON_GAS,
    &MOVE_POISON_POWDER
};

static const std::vector<const Move*> sleepMoves = {
    &MOVE_SPORE
};

static const std::vector<const Move*> paralysisMoves = {
    &MOVE_THUNDER_WAVE
};

static const std::vector<const Move*> burnMoves = {
    &MOVE_WILL_O_WISP
};

static const Move* pickBestPoisoningMove(const std::unordered_set<const Move*>& myMoves){
    for (auto move : poisoningMoves){
        if (myMoves.contains(move)){
            return move;
        }
    }
    return nullptr;
}

static const Move* pickBestSleepMove(const std::unordered_set<const Move*>& myMoves){
    for (auto move : sleepMoves){
        if (myMoves.contains(move)){
            return move;
        }
    }
    return nullptr;
}

static const Move* pickBestParalysisMove(const std::unordered_set<const Move*>& myMoves){
    for (auto move : paralysisMoves){
        if (myMoves.contains(move)){
            return move;
        }
    }
    return nullptr;
}

static const Move* pickBestBurnMove(const std::unordered_set<const Move*>& myMoves){
    for (auto move : burnMoves){
        if (myMoves.contains(move)){
            return move;
        }
    }
    return nullptr;
}

TrainerInfo::TrainerInfo(const std::string& name, TrainerLevel level) : trainerLevel{level}, name{name}
{
}
static void findMostDamagingMove(Pokemon* myPoke, Pokemon* enemyPoke, const std::unordered_set<const Move*>& myMoves, const Move*& outMove, int& outDamage){
    for (auto move : myMoves){
        if (move->damageCategory == DamageCategory::STATUS || move->power == 0){
            continue;
        }
        if (move->flatDamage > 0 && move->flatDamage > outDamage && typeMatchup(move->type, enemyPoke->currentType[0], enemyPoke->currentType[1]) > 0.0f){
            outDamage = move->flatDamage;
            outMove = move;
            continue;
        }
        MoveUse moveUse(move, myPoke, enemyPoke, myPoke->battle);
        int avgDamage = MoveFunctions::calculateDirectDamage(&moveUse, true).damage;
        if (avgDamage > outDamage){
            outMove = move;
            outDamage = avgDamage;
        }
    }
}

static void removeSporeBasedMoves(std::unordered_set<const Move*>& moves){
    tflib::static_vector<const Move*, 4> toRemove;
    for (auto move : moves){
        if (move->sporeBased){
            toRemove.push_back(move);
        }
    }
    for (auto move : toRemove){
        moves.erase(move);
    }
}

static float typePoints(Pokemon* myPoke, Pokemon* enemyPoke){
    float myOffense = typeMatchup(myPoke->currentType[0], enemyPoke->currentType[0], enemyPoke->currentType[1]) + typeMatchup(myPoke->currentType[1], enemyPoke->currentType[0], enemyPoke->currentType[1]);
    float myDefense = typeMatchup(enemyPoke->currentType[0], myPoke->currentType[0], myPoke->currentType[1]) + typeMatchup(enemyPoke->currentType[1], myPoke->currentType[0], myPoke->currentType[1]);
    return myOffense - myDefense;
}

static const Move* pickSmartMove(Pokemon* myPoke, Pokemon* enemyPoke,  Battle* battle){
    std::unordered_set<const Move*> validMoves;
    bool isPlayer1 = myPoke == battle->player1ActivePokemon;
    auto& myTeam = isPlayer1 ?  battle->player1Team : battle->player2Team;
    int& switchCounter = isPlayer1 ? battle->player1SwitchCounter : battle->player2SwitchCounter;
    for (int i = 0; i < myPoke->currentMoves.size(); i++){
        const Move* move = myPoke->currentMoves[i];
        if (move != &MOVE_NONE && myPoke->currentPP[i] > 0 && !myPoke->isMoveDisabled(move)){
            validMoves.insert(move);
        }
    }
    const Move* mostDamagingMove = nullptr;
    int mostDamage = 0;
    findMostDamagingMove(myPoke, enemyPoke, validMoves, mostDamagingMove, mostDamage);
    float mostDamagePercent = ((float) mostDamage / enemyPoke->getStat(Stat::HP)) * 100.0f;

    float myHealthPercent = ((float) myPoke->currentHealth / myPoke->getStat(Stat::HP)) * 100.0f;
    float enemyHealthPercent = ((float) enemyPoke->currentHealth / enemyPoke->getStat(Stat::HP)) * 100.0f;

    bool imFaster = myPoke->getStat(Stat::SPEED) > enemyPoke->getStat(Stat::SPEED);
    int hitsToKO = mostDamage ? enemyPoke->currentHealth / mostDamage + 1 : INT_MAX;

    if (enemyPoke->isType(Type::GRASS)){
        removeSporeBasedMoves(validMoves);
    }

    const Move* bestPoisoningMove = pickBestPoisoningMove(validMoves);
    const Move* bestSleepMove = pickBestSleepMove(validMoves);
    const Move* bestParalysisMove = pickBestParalysisMove(validMoves);
    const Move* bestBurnMove = pickBestBurnMove(validMoves);

    bool guranteedProtect = !myPoke->hasVolatile(&VOLATILE_PROTECT_STATE) || (myPoke->getVolatileState<ProtectState>(&VOLATILE_PROTECT_STATE).protectsInARow == 0);

    bool haveResidualHealing = myPoke->getCurrentItem() == &ITEM_LEFTOVERS || (myPoke->isType(Type::POISON) && myPoke->getCurrentItem() == &ITEM_BLACK_SLUDGE) || enemyPoke->hasVolatile(&VOLATILE_LEECH_SEED);

    bool enemyPokeTwoTurnAttack = enemyPoke->nextMove != nullptr;

    bool enemyIsSemiInvulnerable = (enemyPoke->hasVolatile(&VOLATILE_FLYING) && !mostDamagingMove->hitsFly) || (enemyPoke->hasVolatile(&VOLATILE_DIGGING) && !mostDamagingMove->hitsDig);

    if (mostDamagingMove && mostDamage >= enemyPoke->currentHealth && (imFaster && !enemyIsSemiInvulnerable)){
        return mostDamagingMove;
    }
    else if ( // Need to meet the requirements to protect and have at least one reason to use it.
        
        //Requirements for protect
        (validMoves.contains(&MOVE_PROTECT) && 
        guranteedProtect && 
        !(enemyPoke->getStatus() == &STATUS_SLEEP)) && 
        (
        //Reasons to use protect
        (enemyPokeTwoTurnAttack) || 
        (myHealthPercent <= 88.0f && haveResidualHealing) ||
        (enemyPoke->getStatus() == &STATUS_POISON || (enemyPoke->getStatus() == &STATUS_BURN && enemyPoke->getCurrentItem() != &ITEM_LEFTOVERS) || enemyPoke->getStatus() == &STATUS_BAD_POISON || enemyPoke->hasVolatile(&VOLATILE_LEECH_SEED))
    )){
        return &MOVE_PROTECT;
    }
    else if (validMoves.contains(&MOVE_STEALTH_ROCK) && hitsToKO > 1 && !battle->sideHasFieldEffect(!isPlayer1, &FIELD_EFFECT_STEALTH_ROCK)){
        return &MOVE_STEALTH_ROCK;
    }
    else if (validMoves.contains(&MOVE_LEECH_SEED) && hitsToKO > 2 && !enemyPoke->hasVolatile(&VOLATILE_LEECH_SEED)){
        return &MOVE_LEECH_SEED;
    }
    else if (bestSleepMove && hitsToKO > 3 && enemyPoke->getStatus() == &STATUS_NONE){
        return bestSleepMove;
    }
    else if (bestPoisoningMove && hitsToKO > 3 && enemyPoke->getStatus() == &STATUS_NONE && !enemyPoke->isType(Type::POISON)){
        return bestPoisoningMove;
    }
    else if (bestParalysisMove && hitsToKO > 3 && enemyPoke->getStatus() == &STATUS_NONE && !enemyPoke->isType(Type::ELECTRIC)){
        return bestParalysisMove;
    }
    else if (bestBurnMove && hitsToKO > 3 && enemyPoke->getStatus() == &STATUS_NONE && !enemyPoke->isType(Type::FIRE)){
        return bestBurnMove;
    }
    else if (validMoves.contains(&MOVE_REFLECT) && !battle->sideHasFieldEffect(isPlayer1, &FIELD_EFFECT_REFLECT) && hitsToKO > 2){
        return &MOVE_REFLECT;
    }
    else if (mostDamagingMove){
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
        if (move != &MOVE_NONE && myPoke->currentPP[i] > 0 && !myPoke->isMoveDisabled(move)){
            validMoves.push_back(move);
        }
    }

    switch (trainerLevel)
    {
    case TrainerLevel::FIRST_MOVE:
        if (validMoves.size() == 0){
            return &MOVE_STRUGGLE;
        }
        return validMoves[0];
    case TrainerLevel::SWITCHER:
        if (getValidSwitchesCount(myPoke, battle) > 0){
            return &MOVE_SWITCH;
        }
        if (validMoves.size() <= 0){
            return &MOVE_STRUGGLE;
        }
        return validMoves[battle->randInt(0, validMoves.size())];
    case TrainerLevel::WILD:
        if (validMoves.size() <= 0){
            return &MOVE_STRUGGLE;
        }
        return validMoves[battle->randInt(0, validMoves.size())];
    case TrainerLevel::USE_2_MOVES_THEN_SWITCH:
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
    case TrainerLevel::TRAINER:{
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
    case TrainerLevel::BOSS:
        if (validMoves.size() <= 0){
            return &MOVE_STRUGGLE;
        }
        return pickSmartMove(myPoke, enemyPoke, battle);
    default:
        battle->assertTrue(false, "Unimplemented trainer level: " + std::to_string((int)trainerLevel));
    }
    return nullptr;
}

int TrainerInfo::pickPokemon(Pokemon* currentlyActivePokemon, Pokemon* enemyPoke, Battle* battle) const{
    auto& myTeam = this == battle->getPlayer1() ? battle->player1Team : battle->player2Team;
    tflib::static_vector<int,5> validSlots;
    getValidSwitches(currentlyActivePokemon, battle, validSlots);
    battle->assertTrue(validSlots.size() > 0, "pickPokemon called without any valid pokemon to switch to.");

    switch (trainerLevel)
    {
    case TrainerLevel::BOSS:
    case TrainerLevel::TRAINER:{
        tflib::static_vector<float,5> scores;
        for (auto slot : validSlots){
            Pokemon* poke = &myTeam[slot];
            float levelScore = (float) poke->level / enemyPoke->level;
            float healthScore = (float) poke->currentHealth / (float) poke->getStatRaw(Stat::HP);
            float statusScore = poke->getStatus() == &STATUS_NONE ? 1.0f : 0.75f;
            float typeScore = typePoints(poke, enemyPoke);
            scores.push_back(levelScore * typeScore * healthScore * statusScore);
        }
        float bestScore = std::numeric_limits<float>::lowest();
        int bestScoreIndex = -1;
        for (int i = 0; i < scores.size(); i++){
            if (scores[i] > bestScore){
                bestScoreIndex = i;
                bestScore = scores[i];
            }
        }
        battle->assertTrue(bestScoreIndex >= 0, "Didn't pick a pokemon correctly based on score.");
        return validSlots[bestScoreIndex];
    } break;
    default:{
        return validSlots[battle->randInt(0, validSlots.size())];
    } break;
    }
}

void TrainerInfo::getValidSwitches(Pokemon* currentlyActivePokemon, Battle* battle, tflib::static_vector<int,5>& outVec) const{
    auto& myTeam = this == battle->getPlayer1() ? battle->player1Team : battle->player2Team;
    if (currentlyActivePokemon->isTrapped() && !currentlyActivePokemon->isDead) return;
    for (int i = 0; i < myTeam.size(); i++){
        if (!myTeam[i].empty && !myTeam[i].isDead && currentlyActivePokemon != &(myTeam[i])){
            outVec.push_back(i);
        }
    }
}

int TrainerInfo::getValidSwitchesCount(Pokemon* currentlyActivePokemon, Battle* battle) const{
    auto& myTeam = this == battle->getPlayer1() ? battle->player1Team : battle->player2Team;
    if (currentlyActivePokemon->isTrapped() && !currentlyActivePokemon->isDead) return 0;
    int result = 0;
    for (int i = 0; i < myTeam.size(); i++){
        if (!myTeam[i].empty && !myTeam[i].isDead && currentlyActivePokemon != &(myTeam[i])){
            result++;
        }
    }
    return result;
}

