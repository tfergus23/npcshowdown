#pragma once
#include "battle/Battle.hpp"

class Effect{
public:
    virtual void beforeMove(MoveUse* moveUse,Pokemon* subject) const {}
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
    Effect();
};