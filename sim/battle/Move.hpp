#pragma once
#include <string>
#include "battle/Type.hpp"
#include "battle/DamageCategory.hpp"
#include "battle/TargetType.hpp"
#include "battle/SecondaryEffect.hpp"
#include "battle/MoveUse.hpp"



class Move{
public:
    int id;
    int maxPP;
    std::string name;
    int power;
    int accuracy;
    int priority;
    Type type;
    int critRatio;
    DamageCategory damageCategory;
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
    
    virtual void beforeChecks(MoveUse* myMove, MoveUse* opponentMove){}
    virtual void afterChecks(MoveUse* myMove, MoveUse* opponentMove){}

};