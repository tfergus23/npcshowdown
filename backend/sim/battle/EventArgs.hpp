#pragma once
#include "sim/battle/Pokemon.hpp"
#include "sim/battle/Battle.hpp"
#include "sim/battle/MoveUse.hpp"

class EventArgs{
public:
    Pokemon* const eventSubject; // Subject of event
    MoveUse* const moveUse; //MoveUse if applicable
    EventArgs(Pokemon* eventSubject, MoveUse* moveUse) : eventSubject{eventSubject}, moveUse{moveUse}{}
};