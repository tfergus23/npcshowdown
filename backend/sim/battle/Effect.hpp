#pragma once
#include <string>
#include "sim/battle/Observer.hpp"


class Effect {
public:
    Observer observer;
    std::string name;
    std::string was;
};