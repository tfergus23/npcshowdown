#include "sim/battle/Battle.hpp"
#include "sim/data/Abilities.hpp"
#include <cmath>
#include "sim/data/Statuses.hpp"
#include "sim/battle/MoveUse.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;


const std::string ABILITY_NONE_NAME = "";
const Ability* ABILITY_NONE = nullptr;



const Ability ABILITY_GUTS = {
    .name = "Guts",
    .modifySubjectStat = [](Stat stat,int statVal,Pokemon* subject){
        if (stat == ATTACK && subject->getStatus() != STATUS_NONE){
            float newVal = (float) statVal * 1.5f;
            subject->battle->debug("Guts active");
            return (int) floor(newVal);
        } 
        return statVal;
    }
};

const Ability ABILITY_TORRENT = {
    .observer = {
    .beforeMove = [](Pokemon* subject, Battle* battle, const EventArgs& args){
        //TODO
        if (args.moveUse->user == subject && (float) args.moveUse->user->currentHealth / (float) args.moveUse->user->getStat(HP) <= (1.0f / 3.0f) && args.moveUse->move->type == WATER){
            args.moveUse->damageMod *= 1.5f;
        }
    }
    },
    .name = "Torrent"
};





//Mapping string of name to ability
const std::unordered_map<std::string,const Ability*> abilities = {
    {ABILITY_NONE_NAME, ABILITY_NONE},
    {ABILITY_GUTS.name, &ABILITY_GUTS},
    {ABILITY_TORRENT.name, &ABILITY_TORRENT}
};

const Ability* abilityFromString(const std::string& abilityName){
    return abilities.at(abilityName);
}

std::string createAbilityDataResponse(){
    std::vector<std::string> abilityNames;
    for (auto [name,ptr] : abilities){
        if (name != ABILITY_NONE_NAME){
            abilityNames.push_back(name);
        }
    }
    std::sort(abilityNames.begin(), abilityNames.end());
    json response;
    response["success"] = true;
    response["message"] = "OK";
    response["data"] = abilityNames;
    return response.dump();
}