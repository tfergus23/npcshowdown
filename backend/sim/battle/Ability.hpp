#pragma once
#include "sim/battle/Observer.hpp"
#include "sim/battle/Stat.hpp"
#include "sim/battle/Pokemon.hpp"

class Ability : public Observer{
public:
    std::string name;
    int critMod = 0;
    bool ignorable = false;
    float accuracyMultiplier = 1.0f;
    virtual int modifySubjectStat(Stat stat,int statVal,Pokemon* subject) const {return statVal;}
protected:
    //virtual void beforeMove(Pokemon* subject, Battle* battle, const EventArgs& args) const {}
};
