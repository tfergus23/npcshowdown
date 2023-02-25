#include "sim/utils/ability_factory.hpp"
#include <functional>

/*
std::unordered_map<std::string, std::function<Ability()>> abilityConstructors = {
    {"Torrent", [](){return Ability();}}
};

Ability createAbility(const std::string &name){
    return abilityConstructors[name]();
}

void setAbilityNames(){
    for (auto kv: abilityConstructors){
        abilityNames.push_back(kv.first);
    }
}
*/

