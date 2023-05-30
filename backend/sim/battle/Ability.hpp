#pragma once
#include "sim/battle/Observer.hpp"
#include "sim/battle/Stat.hpp"
#include "sim/battle/Pokemon.hpp"

class Ability {
public:
    Observer observer;
    std::string name;
    int critMod = 0;
    bool ignorable = false;
    float accuracyMultiplier = 1.0f;
    int (*modifySubjectStat)(Stat,int,Pokemon*) = [](Stat stat, int statVal, Pokemon* subject){return statVal;};
protected:
};
