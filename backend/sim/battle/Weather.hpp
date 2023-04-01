#pragma once
#include "sim/battle/Battle.hpp"

class Weather{
public:
    std::string beginText;
    virtual void initializeState(EffectState* state) const {}
    virtual void beforeMove(MoveUse* moveUse) const {}
    virtual void endOfTurn(Battle* battle) const {}
    virtual void priorityEndOfTurn(Battle* battle) const {}
    virtual void afterMove(MoveUse* moveUse) const {}
};