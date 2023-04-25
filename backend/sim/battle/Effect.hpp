#pragma once
#include <string>
#include "sim/battle/Observer.hpp"


class Effect : public Observer{
public:
    std::string name;
};