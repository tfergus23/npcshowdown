#pragma once
#include "sim/battle/Battle.hpp"

class Item{
public:
    std::string name;
    int critMod = 0;
    bool ignorable = false;
    float accuracyMultiplier = 1.0f;

    virtual void initializeState(EffectState* state) const {}
    virtual void beforeMove(MoveUse* moveUse,Pokemon* subject) const {}
    virtual void endOfTurn(Pokemon* subject) const {}
    virtual void priorityEndOfTurn(Pokemon* subject) const {}
    virtual void afterMove(MoveUse* moveUse, Pokemon* subject) const {}
    virtual void onSubjectSwitch(Pokemon* subject) const {}
    virtual void onSubjectEnter(Pokemon* subject) const {}
    virtual void onSubjectDeath(Pokemon* subject) const {}
    virtual void onSubjectAttack(MoveUse* moveUse) const {}
    virtual void onSubjectAttacked(MoveUse* moveUse) const {}
    Item();
};