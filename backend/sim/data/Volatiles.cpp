#include "sim/data/Volatiles.hpp"
#include "sim/battle/Battle.hpp"

const Volatile VOLATILE_NONE;

const Volatile VOLATILE_CONFUSED = {
    .name = "Confused",
    .was = " became confused!"
};

const Volatile VOLATILE_ROOST = {
    .observer = {
    .initialize = [](Pokemon* subject, Battle* battle){
        if (!subject->isType(Type::FLYING)) return;
        
        Type otherType = subject->currentType[0] == Type::FLYING ? subject->currentType[1] : subject->currentType[0];

        subject->currentType = {otherType, Type::NONE};
    },
    .endOfTurn = [](Pokemon* subject, Battle* battle, const EventArgs& e){
        subject->currentType = subject->species->type;
        subject->removeVolatile(&VOLATILE_ROOST);
    }
    },
    .name = "Roost",
};