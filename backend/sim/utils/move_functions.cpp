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
    int critStage = moveUse->move->critRatio + ((moveUse->user->getCurrentItem() != &ITEM_NONE) ? moveUse->user->getCurrentItem()->critMod : 0) + moveUse->user->getCurrentAbility()->critMod + moveUse->user->triggeredCritMod;
    float critChance = critChanceFromStage(critStage);
    float critMod = 1.0f;
    bool crit = false;
    if (!average && moveUse->canCrit && moveUse->move->damageCategory != DamageCategory::STATUS && moveUse->battle->randInt(1,10001) < critChance*100){
        critMod = 1.5f;
        crit = true;
    }
    float typeMod = typeMatchup(moveUse->effectiveType, moveUse->target->currentType[0], moveUse->target->currentType[1]);
    float stabMod = (moveUse->user->isType(moveUse->effectiveType)) ? 1.5f : 1.0f;
    float random = (average) ? 92.5f / 100.0f : (float) moveUse->battle->randInt(85,101) / 100.0f;
    int damage = (int) ceil(calculateDamageBeforeMods(moveUse, crit) * moveUse->damageMod * stabMod * typeMod * critMod * random);
    DealtDamage result;
    result.damage = damage;
    result.typeMod = typeMod;
    result.crit = crit;
    return result;
}

int dealDirectDamage(MoveUse* moveUse, bool logEffectiveness){
    if (!moveUse->canDealDamage){
        if (!moveUse->loggedFailure){
            moveUse->battle->log(moveUse->getFailMessage());
            moveUse->loggedFailure = true;
        }
        return 0;
    }
    DealtDamage dealtDamage = calculateDirectDamage(moveUse);
    dealtDamage.damage = dealDamage(dealtDamage.damage, moveUse);
    if (dealtDamage.damage > 0){
        if (logEffectiveness){
            moveUse->battle->assertTrue(dealtDamage.typeMod != NOT_EFFECTIVE, "Move tried to deal damage when NOT_EFFECTIVE");
            if (dealtDamage.typeMod == SUPER_EFFECTIVE) moveUse->battle->log("It's Super Effective!");
            else if (dealtDamage.typeMod == ULTRA_EFFECTIVE) moveUse->battle->log("It's ULTRA Effective!");
            else if (dealtDamage.typeMod == NOT_VERY_EFFECTIVE) moveUse->battle->log("It's not very effective...");
            else if (dealtDamage.typeMod == BARELY_EFFECTIVE) moveUse->battle->log("It's barely effective...");
            else if (dealtDamage.typeMod == 1.0f);
            else moveUse->battle->assertTrue(false, "Looks like we got a rounding error on our hands boys: " + std::to_string(dealtDamage.typeMod));
        }
        if (dealtDamage.crit){
            moveUse->battle->log("Critical hit!");
        }
    }
    moveUse->battle->raiseEvent(Event::POKEMON_ATTACKED, EventArgs(nullptr, moveUse));
    moveUse->damageDone = dealtDamage.damage;
    return dealtDamage.damage;
}

bool applySecondaryEffect(MoveUse* moveUse, MoveUse* opponentMove){
    float sereneGraceMod = 1.0f;
    if (!(moveUse->battle->randInt(1,101) <= moveUse->move->secondaryEffectChance * sereneGraceMod && moveUse->damageDone > 0 && moveUse->target->currentHealth > 0)) return false;
    switch (moveUse->move->secondaryEffect)
    {
    case SecondaryEffect::POISON:
        return applyStatus(&STATUS_POISON, moveUse, false);
    case SecondaryEffect::PARALYZE:
        return applyStatus(&STATUS_PARALYSIS, moveUse, false);
    case SecondaryEffect::SLEEP:
        return applyStatus(&STATUS_SLEEP, moveUse, false);
    case SecondaryEffect::BURN:
        return applyStatus(&STATUS_BURN, moveUse, false);
    case SecondaryEffect::FREEZE:
        return applyStatus(&STATUS_FROZEN, moveUse, false);
    case SecondaryEffect::BAD_POISON:
        return applyStatus(&STATUS_BAD_POISON, moveUse, false);
    case SecondaryEffect::ATTACK_CHANGE:
        return changeStatModifier(Stat::ATTACK, (int) moveUse->move->secondaryEffectValue, moveUse->target, moveUse->battle, moveUse, false);
    case SecondaryEffect::DEFENSE_CHANGE:
        return changeStatModifier(Stat::DEFENSE, (int) moveUse->move->secondaryEffectValue, moveUse->target, moveUse->battle, moveUse, false);
    case SecondaryEffect::SP_ATTACK_CHANGE:
        return changeStatModifier(Stat::SPATTACK, (int) moveUse->move->secondaryEffectValue, moveUse->target, moveUse->battle, moveUse, false);
    case SecondaryEffect::SP_DEFENSE_CHANGE:
        return changeStatModifier(Stat::SPDEFENSE, (int) moveUse->move->secondaryEffectValue, moveUse->target, moveUse->battle, moveUse, false);
    case SecondaryEffect::SPEED_CHANGE:
        return changeStatModifier(Stat::SPEED, (int) moveUse->move->secondaryEffectValue, moveUse->target, moveUse->battle, moveUse, false);
    case SecondaryEffect::CONFUSE:
    {
        if (moveUse->target->hasEffect(&EFFECT_CONFUSED)) {
            return false;
        }
        bool success = applyEffect(&EFFECT_CONFUSED, moveUse);
        if (success) {
            moveUse->battle->log(moveUse->target->nickname + " became confused!");
        }
        return success;
    }
    case SecondaryEffect::FLINCH:
    {
        if (opponentMove->move != &MOVE_SWITCH) {
            opponentMove->dontStart(moveUse->target->nickname + " flinched!");
            return true;
        }
        return false;
    }
    case SecondaryEffect::TRI_ATTACK:
    {
        int random = moveUse->battle->randInt(0, 3);
        switch (random)
        {
        case 0:
            return applyStatus(&STATUS_PARALYSIS, moveUse, false);
        case 1:
            return applyStatus(&STATUS_FROZEN, moveUse, false);
        case 2:
            return applyStatus(&STATUS_BURN, moveUse, false);
        }
    }
    default:
        return false;
    }
}

int dealFlatDamage(int damage, MoveUse* moveUse) {
    if (!moveUse->canDealDamage) {
        if (!moveUse->loggedFailure) {
            moveUse->battle->log(moveUse->getFailMessage());
            moveUse->loggedFailure = true;
        }
        return 0;
    }
    int damageToDeal = (moveUse->target->currentHealth - damage < 0) ? moveUse->target->currentHealth : damage;
    int damageDone = dealDamage(damageToDeal, moveUse);
    moveUse->damageDone = damageDone;
    moveUse->battle->raiseEvent(Event::POKEMON_ATTACKED, EventArgs(nullptr, moveUse));
    return damageDone;
}
bool selfDestruct(MoveUse* moveUse) {
    if (moveUse->cantSelfDestruct) {
        moveUse->battle->log(moveUse->getFailMessage());
        return false;
    }
    moveUse->user->currentHealth -= moveUse->user->currentHealth;
    moveUse->battle->killTheDead();
    return true;
}

int dealResidualPercentDamage(float percent, Pokemon* target, Battle* battle) {
    int damage = (int) floor(target->getStat(Stat::HP) * (percent / 100.0f));
    if (damage > target->currentHealth) damage = target->currentHealth;
    target->currentHealth -= damage;
    if (damage > 0) battle->log(target->nickname + " took " + std::to_string(damage) + " damage.");
    return damage;
}


int dealPercentDamage(float percent, MoveUse* moveUse) {
    int damage = (int)floor(moveUse->target->getStat(Stat::HP) * (percent / 100.0f));
    if (damage > moveUse->target->currentHealth) damage = moveUse->target->currentHealth;
    int damageDealt = dealDamage(damage, moveUse);
    if (damageDealt > 0) moveUse->battle->log(moveUse->target->nickname + " took " + std::to_string(damage) + " damage.");
    return damageDealt;
}
int giveHealing(int healing, Pokemon* recipient, Battle* battle) {
    int recipientHP = recipient->getStat(Stat::HP);
    int healingToGive = (recipient->currentHealth + healing > recipientHP) ? recipientHP - recipient->currentHealth : healing;
    recipient->currentHealth += healingToGive;
    if (healingToGive > 0) battle->log(recipient->nickname + " was healed for " + std::to_string(healing) + " health.");
    return healingToGive;
}
void givePercentHealing(float percent, Pokemon* recipient, Battle* battle) {
    int recipientHP = recipient->getStat(Stat::HP);
    int healing = (int) floor(recipientHP * (percent / 100.0f));
    giveHealing(healing, recipient, battle);
}
//Not sure why this is like this... will wait and see
void giveFlatHealing(int healing, Pokemon* recipient, Battle* battle) {
    giveHealing(healing, recipient, battle);
}
bool applyStatus(const Status* status, MoveUse* moveUse, bool logTypeFailure) {
    //TODO: Why are these separate if blocks?
    if (moveUse->target->getStatus() != STATUS_NONE) {
        if (logTypeFailure) moveUse->battle->log("But it failed!");
        return false;
    }
    if (!moveUse->canApplyStatus || moveUse->target->isDead) {
        if (!moveUse->loggedFailure) {
            moveUse->battle->log(moveUse->getFailMessage());
            moveUse->loggedFailure = true;
        }
        return false;
    }
    if (status == &STATUS_FROZEN && (moveUse->battle->weather == &WEATHER_SUN || moveUse->target->isType(Type::ICE))) {
        return false;
    }
    if (status == &STATUS_BURN && moveUse->target->isType(Type::FIRE)) {
        if (logTypeFailure) moveUse->battle->log("It doesn't affect " + moveUse->target->nickname + "...");
        return false;
    }
    if ((status == &STATUS_POISON || status == &STATUS_BAD_POISON) && (moveUse->target->isType(Type::POISON) || moveUse->target->isType(Type::STEEL))) {
        if (logTypeFailure) moveUse->battle->log("It doesn't affect " + moveUse->target->nickname + "...");
        return false;
    }
    if (status == &STATUS_PARALYSIS && moveUse->target->isType(Type::ELECTRIC)) {
        if (logTypeFailure) moveUse->battle->log("It doesn't affect " + moveUse->target->nickname + "...");
        return false;
    }
    moveUse->target->applyStatus(status);
    moveUse->battle->log(moveUse->target->nickname + status->was);
    return true;
}
bool applyEffect(const Effect* effect, MoveUse* moveUse) {
    if (!moveUse->canApplyStatus || moveUse->target->isDead) {
        if (!moveUse->loggedFailure) {
            moveUse->battle->log(moveUse->getFailMessage());
            moveUse->loggedFailure = true;
        }
        return false;
    }
    moveUse->target->applyEffect(effect);
    return true;
}
bool changeStatModifier(Stat stat, int change, Pokemon* pokemon, Battle* battle, MoveUse* moveUse, bool logNoChange) {
    int currentMod = pokemon->boosts[(int)stat];
    int actualChange = 0;
    if ((!moveUse->canLowerStats && change < 0) || (!moveUse->canRaiseStats && change > 0)) {
        if (logNoChange) battle->log(moveUse->getFailMessage());
        return false;
    }
    if (change > 0) {
        actualChange = (currentMod + change <= 6) ? change : 6 - currentMod;
    }
    else {
        actualChange = (currentMod + change <= 6) ? change : -6 - currentMod;
    }
    pokemon->boosts[(int)stat] += actualChange;
    if (actualChange == 0 && change != 0) {
        const std::string adjective = (change > 0) ? "higher" : "lower";
        if (logNoChange) battle->log(pokemon->nickname + "'s " + statNames[(int)stat] + " can't go any " + adjective + "!");
        return false;
    }
    else {
        std::string verb = (actualChange > 0) ? "rose" : "fell";
        std::string adverb = (abs(actualChange) > 1) ? " sharply" : "";
        battle->log(pokemon->nickname + "'s " + statNames[(int)stat] + " " + verb + adverb + "!");
        return true;
    }

}
bool changeBattleWeather(const Weather* newWeather, Battle* battle) {
    if (battle->weather != WEATHER_NONE && battle->weather == newWeather) {
        battle->log("But it failed!");
        return false;
    }
    else {
        battle->weather = newWeather;
        if (newWeather != WEATHER_NONE) battle->log(newWeather->beginText);
        return true;
    }
}
int calculateDamageBeforeMods(MoveUse* moveUse, bool crit) {
    int div1 = (int)floor((2 * moveUse->user->level) / 5.0f);
    Stat attackingStat = (Stat)moveUse->move->damageCategory;
    Stat defendingStat = (Stat)((int)(moveUse->move->damageCategory) + 1);
    moveUse->battle->assertTrue(attackingStat == Stat::ATTACK || attackingStat == Stat::SPATTACK, "Incorrectly calculated attackingStat");
    moveUse->battle->assertTrue(defendingStat == Stat::DEFENSE || defendingStat == Stat::SPDEFENSE, "Incorrectly calculated defendingStat");
    int attack = moveUse->user->getStat(attackingStat);
    int defense = moveUse->target->getStat(defendingStat);
    float div2 = (float)attack / (float)defense;
    int div3 = (int)floor(((div1 + 2) * moveUse->effectivePower * div2) / 50.0f);
    int damage = div3 + 2;
    return damage;
}
int calculateDamageBeforeMods(MoveUse* moveUse, bool crit, Stat attackingStat, Stat defendingStat) {
    int div1 = (int)floor((2 * moveUse->user->level) / 5.0f);
    int attack = moveUse->user->getStat(attackingStat);
    int defense = moveUse->target->getStat(defendingStat);
    float div2 = (float)attack / (float)defense;
    int div3 = (int)floor(((div1 + 2) * moveUse->effectivePower * div2) / 50.0f);
    int damage = div3 + 2;
    return damage;
}
bool genderCompatible(Gender gender1, Gender gender2) {
    if (gender1 == Gender::GENDERLESS || gender2 == Gender::GENDERLESS) return false;
    return gender1 != gender2;
}
void crash(Pokemon* user, Battle* battle) {
    battle->log(user->nickname + " kept going and crashed!");
    dealResidualPercentDamage(50.0f, user, battle);
}

int dealDirectDamageWithRecoil(MoveUse* moveUse, float recoilMultiplier, bool logEffectiveness){
    int damage = dealDirectDamage(moveUse, logEffectiveness);
    if (damage > 0){
        int recoil = (int) ceil((float) damage * recoilMultiplier);
        int recoilDamage = (moveUse->user->currentHealth - recoil < 0) ? moveUse->user->currentHealth : recoil;
        moveUse->user->currentHealth -= recoilDamage;
        moveUse->battle->log(moveUse->user->nickname + " took " + std::to_string(recoilDamage) + " damage as recoil!");
    }
    return damage;
}