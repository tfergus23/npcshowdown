#pragma once
#include <string>
#include "Observer.hpp"

class Weather {
public:
    Observer observer;
    std::string beginText;
};