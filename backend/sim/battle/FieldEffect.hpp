#pragma once
#include "sim/battle/Observer.hpp"
#include <string>



class FieldEffect : public Observer{
public:
    std::string name;
};