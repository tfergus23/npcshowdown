#pragma once
#include "sim/battle/Battle.hpp"

class Status{
public:
    virtual void initializeState(EffectState* state) const {}
    virtual void beforeMove(MoveUse* moveUse,Pokemon* subject) const {}
    virtual void endOfTurn(Pokemon* subject) const {}
    virtual void priorityEndOfTurn(Pokemon* subject) const {}
    virtual void afterMove(MoveUse* moveUse, Pokemon* subject) const {}
    virtual void onSubjectSwitch(Pokemon* subject) const {}
    virtual void onSubjectEnter(Pokemon* subject) const {}
};