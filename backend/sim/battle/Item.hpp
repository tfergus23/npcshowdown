#pragma once
#include "sim/battle/Observer.hpp"
#include <string>


class Item : public Observer{
public:
    std::string name;
    int critMod = 0;
    bool ignorable = false;
    float accuracyMultiplier = 1.0f;
};