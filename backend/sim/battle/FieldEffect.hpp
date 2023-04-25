#pragma once
#include "sim/battle/Battle.hpp"
#include "sim/battle/Observer.hpp"



class FieldEffect : public Observer{
public:
    std::string name;
};