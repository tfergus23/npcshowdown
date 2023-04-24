#pragma once
#include "sim/battle/Battle.hpp"
#include "sim/battle/Observer.hpp"

class Effect : public Observer{
public:
    std::string name;
};