#pragma once
#include "sim/battle/Observer.hpp"
#include <string>


class Item {
public:
    Observer observer;
    std::string name;
    int critMod = 0;
    float accuracyMultiplier = 1.0f;
    int16_t id = 0;
    bool ignorable = false;
};