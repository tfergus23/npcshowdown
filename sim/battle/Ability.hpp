#pragma once
#include <string>
#include "battle/MoveUse.hpp"
#include <functional>
#include "battle/Pokemon.hpp"

class Ability{
public:
    std::string name;
    int critMod = 0;
    bool ignorable = false;
    float accuracyMultiplier = 1.0f;

    virtual void beforeMove(MoveUse* moveUse,Pokemon* subject) const {}
    virtual int modifySubjectStat(Stat stat,int statVal,Pokemon* subject) const {return statVal;}
    virtual void endOfTurn(Pokemon* subject) const {}
    virtual void priorityEndOfTurn(Pokemon* subject) const {}
    virtual void afterMove(MoveUse* moveUse, Pokemon* subject) const {}
    virtual void onSubjectSwitch(Pokemon* subject) const {}
    virtual void onSubjectEnter(Pokemon* subject) const {}
    virtual void onSubjectDeath(Pokemon* subject) const {}
    virtual void onSubjectAttack(MoveUse* moveUse) const {}
    virtual void onSubjectAttacked(MoveUse* moveUse) const {}
    virtual void onOpponentSwitch(Pokemon* subject,Pokemon* opponent) const {}
    virtual void onOpponentEnter(Pokemon* subject, Pokemon* opponent) const {}
    Ability();
};