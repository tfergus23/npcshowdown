#pragma once
#include "battle/Battle.hpp"

class Weather{
public:
    std::string beginText;
    virtual void beforeMove(MoveUse* moveUse,Pokemon* subject) const {}
    virtual void endOfTurn(Pokemon* subject) const {}
    virtual void priorityEndOfTurn(Pokemon* subject) const {}
    virtual void afterMove(MoveUse* moveUse, Pokemon* subject) const {}
};