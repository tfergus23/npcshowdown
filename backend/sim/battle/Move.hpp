#pragma once
#include <string>
#include "sim/battle/Type.hpp"
#include "sim/battle/DamageCategory.hpp"
#include "sim/battle/TargetType.hpp"
#include "sim/battle/SecondaryEffect.hpp"
#include <cstdint>

class MoveUse;

class Move{
public:
    std::string name;
    Type type;
    DamageCategory damageCategory;
    int power;
    int accuracy;
    int maxPP;
    int priority;
    int critRatio;
    TargetType targetType;
    SecondaryEffect secondaryEffect;
    float secondaryEffectChance;
    float secondaryEffectValue;
    int flatDamage = 0; // If this is > 0, the move will deal flat damage instead of scaled. 
    int16_t id = -1;

    //Flags
    bool nothingAfterChecks = false;
    bool contact = false;
    bool protect = false;
    bool magicCoat = false;
    bool snatch = false;
    bool mirrorMove = false;
    bool kingsRock = false;
    bool crashOnFail = false;
    bool charged = false;
    bool hitsFly = false;
    bool hitsDig = false;
    bool minimize = false;
    bool breaksScreen = false;
    bool skillLink = false;
    bool soundBased = false;
    bool usableWhileAsleep = false;
    bool bypassSubstitute = false;
    bool sporeBased = false;

    void (*beforeChecks)(MoveUse*, MoveUse*) = [](MoveUse* myMove, MoveUse* opponentMove){};
    void (*afterChecks) (MoveUse*, MoveUse*) = [](MoveUse* myMove, MoveUse* opponentMove){};

};