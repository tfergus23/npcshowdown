#pragma once
#include "sim/battle/Battle.hpp"

class FieldEffect{
public:
    virtual void initializeState(EffectState* state) const {}
    virtual void beforeMove(MoveUse* moveUse) const {}
    virtual void endOfTurn(Battle* battle) const {}
    virtual void priorityEndOfTurn(Battle* battle) const {}
    virtual void afterMove(MoveUse* moveUse) const {}
    virtual void onSubjectSwitch(Pokemon* subject) const {}
    virtual void onSubjectEnter(Pokemon* subject) const {}
    virtual void onSubjectDeath(Pokemon* subject) const {}
    FieldEffect();
};