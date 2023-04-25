#pragma once
#include <string>

class Weather : public Observer{
public:
    std::string beginText;
    Weather();
};