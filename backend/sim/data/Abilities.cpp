#include "sim/battle/Battle.hpp"
#include "sim/data/Abilities.hpp"
#include <cmath>
#include "sim/data/Statuses.hpp"
#include "sim/battle/MoveUse.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

const Ability ABILITY_GUTS = {
    .observer{
    .beforeMove = [](Pokemon* subject, Battle* battle, const EventArgs& args){
        if (subject == args.moveUse->user){
            args.moveUse->guts = true;
        }
    },
    },
    .name = "Guts",
    .modifySubjectStat = [](Stat stat,int statVal,Pokemon* subject){
        if (stat == Stat::ATTACK && subject->getStatus() != STATUS_NONE){
            float newVal = (float) statVal * 1.5f;
            subject->battle->debug("Guts active");
            return (int) floor(newVal);
        } 
        return statVal;
    },
    .id = 0
};

const Ability ABILITY_TORRENT = {
    .observer = {
    .beforeMove = [](Pokemon* subject, Battle* battle, const EventArgs& args){
        //TODO
        if (args.moveUse->user == subject && (float) args.moveUse->user->currentHealth / (float) args.moveUse->user->getStat(Stat::HP) <= (1.0f / 3.0f) && args.moveUse->move->type == Type::WATER){
            args.moveUse->damageMod *= 1.5f;
        }
    }
    },
    .name = "Torrent",
    .id = 1
};





//Mapping string of name to ability
const std::unordered_map<std::string,const Ability*> abilities = {
    {ABILITY_GUTS.name, &ABILITY_GUTS},
    {ABILITY_TORRENT.name, &ABILITY_TORRENT}
};

std::unordered_map<int16_t, const Ability*> idToAbility;

void mapIDToAbility(int16_t id, const Ability* ability){
    assert(!idToAbility.contains(id));
    assert(id == ability->id);
    idToAbility[id] = ability;
}

void mapIDsToAbilities(){
    mapIDToAbility(ABILITY_GUTS.id, &ABILITY_GUTS);
    mapIDToAbility(ABILITY_TORRENT.id, &ABILITY_TORRENT);
}

const Ability* abilityFromString(const std::string& abilityName){
    return abilities.at(abilityName);
}

const Ability* abilityFromId(int16_t id){
    return idToAbility.at(id);
}

std::string createAbilityDataResponse(){
    std::vector<std::string> abilityNames;
    for (auto [name,ptr] : abilities){
        abilityNames.push_back(name);
    }
    std::sort(abilityNames.begin(), abilityNames.end());
    json response;
    response["success"] = true;
    response["message"] = "OK";
    response["data"] = abilityNames;
    return response.dump();
}