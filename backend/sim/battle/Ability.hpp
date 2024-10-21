#pragma once
#include "sim/battle/Observer.hpp"
#include "sim/battle/Stat.hpp"
#include "sim/battle/Pokemon.hpp"

class Ability {
public:
    Observer observer;
    std::string name;
    int critMod = 0;
    float accuracyMultiplier = 1.0f;
    int (*modifySubjectStat)(Stat,int,Pokemon*) = [](Stat stat, int statVal, Pokemon* subject){return statVal;};
    bool ignorable = false;
    short id = -1;
protected:
};
