#pragma once
#include "Battle.hpp"

class FieldEffect{
public:
    virtual void beforeMove(MoveUse* moveUse,Pokemon* subject) const {}
    virtual void endOfTurn(Pokemon* subject) const {}
    virtual void priorityEndOfTurn(Pokemon* subject) const {}
    virtual void afterMove(MoveUse* moveUse, Pokemon* subject) const {}
    virtual void onSubjectSwitch(Pokemon* subject) const {}
    virtual void onSubjectEnter(Pokemon* subject) const {}
    virtual void onSubjectDeath(Pokemon* subject) const {}
    FieldEffect();
};