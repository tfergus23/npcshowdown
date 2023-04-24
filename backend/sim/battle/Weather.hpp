#pragma once
#include "sim/battle/Battle.hpp"
#include "sim/battle/Observer.hpp"

class Weather : public Observer{
public:
    std::string beginText;
    Weather();
};