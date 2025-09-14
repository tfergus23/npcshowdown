#pragma once
#include "sim/battle/MoveUse.hpp"
#include "sim/battle/Battle.hpp"

struct DealtDamage{
    int damage;
    bool crit;
    float typeMod;
};

DealtDamage calculateDirectDamage(MoveUse* moveUse, bool average);
int dealDirectDamage(MoveUse* moveUse, bool logEffectiveness = true);
bool applySecondaryEffect(MoveUse* moveUse, MoveUse* opponentMove);
int dealFlatDamage(int damage, MoveUse* moveUse);
bool selfDestruct(MoveUse* moveUse);
int dealResidualPercentDamage(float percent, Pokemon* target, Battle* battle);
int dealPercentDamage(float percent, MoveUse* moveUse);
void givePercentHealing(float percent, Pokemon* recipient, Battle* battle);
void giveFlatHealing(int healing, Pokemon* recipient, Battle* battle);
bool applyStatus(const Status* status, MoveUse* moveUse, bool logFailure=false);
bool applyEffect(const Effect* effect, MoveUse* moveUse, bool logFailure=true);
bool changeStatModifier(Stat stat, int change, Pokemon* pokemon, Battle* battle, MoveUse* moveUse, bool logNoChange=true);
bool changeBattleWeather(const Weather* newWeather, Battle* battle);
int calculateDamageBeforeMods(MoveUse* moveUse, bool crit);
int calculateDamageBeforeMods(MoveUse* moveUse, bool crit, Stat attackingStat, Stat defendingStat);
bool genderCompatible(Gender gender1, Gender gender2);
void crash(Pokemon* user, Battle* battle);
int dealDirectDamageWithRecoil(MoveUse* moveUse, float recoilMultiplier, bool logEffectiveness = true);