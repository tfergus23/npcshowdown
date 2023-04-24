#pragma once
#include "sim/battle/Battle.hpp"
#include "sim/battle/Observer.hpp"

class Status : public Observer{
public:
    std::string was;
};