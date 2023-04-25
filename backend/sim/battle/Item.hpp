#pragma once
#include "sim/battle/Battle.hpp"
#include "sim/battle/Observer.hpp"


class Item : public Observer{
public:
    std::string name;
    int critMod = 0;
    bool ignorable = false;
    float accuracyMultiplier = 1.0f;
};