#include "move_functions.hpp"
#include "sim/data/FieldEffects.hpp"
#include "sim/data/Items.hpp"
#include "sim/utils/stage_multipliers.hpp"
#include "sim/battle/Type.hpp"
#include "sim/data/Effects.hpp"
#include "sim/data/Moves.hpp"

int dealDamage(int damage, MoveUse* moveUse){
    bool isPlayer1 = moveUse->target == moveUse->battle->player1ActivePokemon;
    if (moveUse->battle->sideHasFieldEffect(isPlayer1, &FIELD_EFFECT_SUBSTITUTE)){
        EffectState* subState = moveUse->battle->getFieldEffectState(isPlayer1, &FIELD_EFFECT_SUBSTITUTE);
        if (damage > subState->substituteHealth) damage = subState->substituteHealth;
        subState->substituteHealth -= damage;
        if (damage > 0) moveUse->battle->log(moveUse->target->nickname + "'s Substitute absorbed the attack!");
    }
    else{
        if (damage >= moveUse->target->currentHealth) damage = moveUse->canKill ? moveUse->target->currentHealth : moveUse->target->currentHealth -1;
        moveUse->target->currentHealth -= damage;
        if (damage > 0) moveUse->battle->log(moveUse->target->nickname + " took " + std::to_string(damage) + " damage!");
    }
    return damage;
}

DealtDamage calculateDirectDamage(MoveUse* moveUse, bool average){
    int critStage = moveUse->move->critRatio + ((moveUse->user->getCurrentItem() != ITEM_NONE) ? moveUse->user->getCurrentItem()->critMod : 0) + moveUse->user->getCurrentAbility()->critMod + moveUse->user->triggeredCritMod;
    float critChance = critChanceFromStage(critStage);
    float critMod = 1.0f;
    bool crit = false;
    if (moveUse->canCrit && moveUse->move->damageCategory != STATUS && moveUse->battle->randInt(1,10001) < critChance*100){
        critMod = 1.5f;
        crit = true;
    }
    float typeMod = typeMatchup(moveUse->move->type, moveUse->target->currentType[0], moveUse->target->currentType[1]);
    float stabMod = (moveUse->user->isType(moveUse->move->type)) ? 1.5f : 1.0f;
    float random = (average) ? 92.5f / 100.0f : (float) moveUse->battle->randInt(85,101) / 100.0f;
    int damage = (int) ceil(calculateDamageBeforeMods(moveUse, crit) * moveUse->damageMod * stabMod * typeMod * critMod * random);
    DealtDamage result;
    result.damage = damage;
    result.typeMod = typeMod;
    result.crit = crit;
    return result;
}

int dealDirectDamage(MoveUse* moveUse, bool logEffectiveness = true){
    if (!moveUse->canDealDamage){
        if (!moveUse->loggedFailure){
            moveUse->battle->log(moveUse->failMessage);
            moveUse->loggedFailure = true;
        }
        return 0;
    }
    DealtDamage dealtDamage = calculateDirectDamage(moveUse);
    dealtDamage.damage = dealDamage(dealtDamage.damage, moveUse);
    if (dealtDamage.damage > 0){
        if (logEffectiveness){
            moveUse->battle->assert(dealtDamage.typeMod != NOT_EFFECTIVE, "Move tried to deal damage when NOT_EFFECTIVE");
            if (dealtDamage.typeMod == SUPER_EFFECTIVE) moveUse->battle->log("It's Super Effective!");
            else if (dealtDamage.typeMod == ULTRA_EFFECTIVE) moveUse->battle->log("It's ULTRA Effective!");
            else if (dealtDamage.typeMod == NOT_VERY_EFFECTIVE) moveUse->battle->log("It's not very effective...");
            else if (dealtDamage.typeMod == BARELY_EFFECTIVE) moveUse->battle->log("It's barely effective...");
            else if (dealtDamage.typeMod == 1.0f);
            else moveUse->battle->assert(false, "Looks like we got a rounding error on our hands boys: " + std::to_string(dealtDamage.typeMod));
        }
        if (dealtDamage.crit){
            moveUse->battle->log("Critical hit!");
        }
    }
    moveUse->battle->raiseEvent(POKEMON_ATTACKED, EventArgs(nullptr, moveUse));
    moveUse->damageDone = dealtDamage.damage;
    return dealtDamage.damage;
}

bool applySecondaryEffect(MoveUse* moveUse, MoveUse* opponentMove){
    float sereneGraceMod = 1.0f;
    if (!(moveUse->battle->randInt(1,101) <= moveUse->move->secondaryEffectChance * sereneGraceMod && moveUse->damageDone > 0 && moveUse->target->currentHealth > 0)) return false;
    switch (moveUse->move->secondaryEffect)
    {
    case NORMAL_POISON:
        return applyStatus(&STATUS_POISON, moveUse, false);
    case PARALYZE:
        return applyStatus(&STATUS_PARALYSIS, moveUse, false);
    case SLEEP:
        return applyStatus(&STATUS_SLEEP, moveUse, false);
    case BURN:
        return applyStatus(&STATUS_BURN, moveUse, false);
    case FREEZE:
        return applyStatus(&STATUS_FROZEN, moveUse, false);
    case BAD_POISON:
        return applyStatus(&STATUS_BAD_POISON, moveUse, false);
    case ATTACK_CHANGE:
        return changeStatModifier(ATTACK, (int) moveUse->move->secondaryEffectValue, moveUse->target, moveUse->battle, moveUse, false);
    case DEFENSE_CHANGE:
        return changeStatModifier(DEFENSE, (int) moveUse->move->secondaryEffectValue, moveUse->target, moveUse->battle, moveUse, false);
    case SP_ATTACK_CHANGE:
        return changeStatModifier(SPATTACK, (int) moveUse->move->secondaryEffectValue, moveUse->target, moveUse->battle, moveUse, false);
    case SP_DEFENSE_CHANGE:
        return changeStatModifier(SPDEFENSE, (int) moveUse->move->secondaryEffectValue, moveUse->target, moveUse->battle, moveUse, false);
    case SPEED_CHANGE:
        return changeStatModifier(SPEED, (int) moveUse->move->secondaryEffectValue, moveUse->target, moveUse->battle, moveUse, false);
    case CONFUSE:
        if (moveUse->target->hasEffect(&EFFECT_CONFUSED)){
            return false;
        }
        bool success = applyEffect(&EFFECT_CONFUSED, moveUse);
        if (success){
            moveUse->battle->log(moveUse->target->nickname + " became confused!");
        }
        return success;
    case FLINCH:
        if (opponentMove->move != &MOVE_SWITCH){
            opponentMove->dontStart(moveUse->target->nickname + " flinched!");
            return true;
        }
        return false;
    case TRI_ATTACK:
        int random = moveUse->battle->randInt(0,3);
        switch (random)
        {
        case 0:
            return applyStatus(&STATUS_PARALYSIS, moveUse, false);
        case 1:
            return applyStatus(&STATUS_FROZEN, moveUse, false);
        case 2:
            return applyStatus(&STATUS_PARALYSIS, moveUse, false);
        }
    default:
        return false;
    }
}

int dealFlatDamage(int damage, MoveUse* moveUse);
void selfDestruct(MoveUse* moveUse);
int dealResidualPercentDamage(float percent, Pokemon* target, Battle* battle);
int dealPercentDamage(float percent, Pokemon* target, Battle* battle);
void givePercentHealing(float percent, Pokemon* recipient, Battle* battle);
void giveFlatHealing(int healing, Pokemon* recipient, Battle* battle);
bool applyStatus(const Status* status, MoveUse* moveUse, bool logTypeFailure=false);
bool applyEffect(const Effect* effect, MoveUse* moveUse);
bool changeStatModifier(Stat stat, int change, Pokemon* pokemon, Battle* battle, MoveUse* moveUse, bool logNoChange=true);
bool changeBattleWeather(const Weather* newWeather, Battle* battle);
int calculateDamageBeforeMods(MoveUse* moveUse, bool crit);
int calculateDamageBeforeMods(MoveUse* moveUse, bool crit, Stat attackingStat, Stat defendingStat);
bool genderCompatible(Gender gender1, Gender gender2);
void crash(Pokemon* user, Battle* battle);