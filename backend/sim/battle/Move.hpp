#pragma once
#include <string>
#include "sim/battle/Type.hpp"
#include "sim/battle/DamageCategory.hpp"
#include "sim/battle/TargetType.hpp"
#include "sim/battle/SecondaryEffect.hpp"

class MoveUse;

/*
    .name = "Pound",
    .type = Type::NORMAL,
    .damageCategory = DamageCategory::PHYSICAL,
    .power = 40,
    .accuracy = 100,
    .maxPP = 56,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::NOEFFECT,
    .secondaryEffectChance = -1,
    .secondaryEffectValue = -1,
*/

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

    //Flags (Tags)
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
    bool dealsFlatDamage = false;
    bool skillLink = false;
    bool soundBased = false;

    void (*beforeChecks)(MoveUse*, MoveUse*) = [](MoveUse* myMove, MoveUse* opponentMove){};
    void (*afterChecks) (MoveUse*, MoveUse*) = [](MoveUse* myMove, MoveUse* opponentMove){};

};