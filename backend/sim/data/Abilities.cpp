#include "sim/battle/Battle.hpp"
#include "sim/data/Abilities.hpp"
#include <cmath>
#include "sim/data/Statuses.hpp"
#include "sim/battle/MoveUse.hpp"
#include <nlohmann/json.hpp>
#include "Moves.hpp"
using json = nlohmann::json;

const Ability ABILITY_GUTS = {
    .observer{
    .modifySubjectStat = [](Stat stat, Pokemon* subject){
        if (stat == Stat::ATTACK && subject->getStatus() != &STATUS_NONE){
            return 1.5f;
        } 
        return 1.0f;
    }
    },
    .name = "Guts",
    .id = 62
};

const Ability ABILITY_TORRENT = {
    .observer = {
    .beforeMove = [](Pokemon* subject, Battle* battle, const EventArgs& args){
        //TODO
        if (args.moveUse->user == subject && (float) args.moveUse->user->currentHealth / (float) args.moveUse->user->getStat(Stat::HP) <= (1.0f / 3.0f) && args.moveUse->effectiveType == Type::WATER){
            args.moveUse->damageMod *= 1.5f;
        }
    }
    },
    .name = "Torrent",
    .id = 67
};

const Ability ABILITY_BLAZE = {
    .observer = {
    .beforeMove = [](Pokemon* subject, Battle* battle, const EventArgs& args){
        if (args.moveUse->user == subject && (float) args.moveUse->user->currentHealth / (float) args.moveUse->user->getStat(Stat::HP) <= (1.0f / 3.0f) && args.moveUse->effectiveType == Type::FIRE){
            args.moveUse->damageMod *= 1.5f;
        }
    }
    },
    .name = "Blaze",
    .id = 66
};

const Ability ABILITY_OVERGROW = {
    .observer = {
    .beforeMove = [](Pokemon* subject, Battle* battle, const EventArgs& args){
        if (args.moveUse->user == subject && (float) args.moveUse->user->currentHealth / (float) args.moveUse->user->getStat(Stat::HP) <= (1.0f / 3.0f) && args.moveUse->effectiveType == Type::GRASS){
            args.moveUse->damageMod *= 1.5f;
        }
    }
    },
    .name = "Overgrow",
    .id = 65
};

const Ability ABILITY_TRUANT = {
    .observer = {
    .initialize = [](Pokemon* subject, Battle* battle){
        subject->abilityState.emplace<TruantState>();
    },
    .beforeMove = [](Pokemon* subject, Battle* battle, const EventArgs& args){
        TruantState& state = std::get<TruantState>(subject->abilityState);
        if (args.moveUse->user == subject && state.isTruant && args.moveUse->move != &MOVE_SWITCH){
            args.moveUse->dontStart(subject->nickname + " is loafing around!");
        }
    },
    .afterMove = [](Pokemon* subject, Battle* battle, const EventArgs& args){
        TruantState& state = std::get<TruantState>(subject->abilityState);
        if (args.moveUse->user == subject){
            state.isTruant = !state.isTruant;
        }
    }
    },
    .name = "Truant",
    .id = 54
};

//Mapping string of name to ability
const std::unordered_map<std::string,const Ability*> abilities = {
    {ABILITY_GUTS.name, &ABILITY_GUTS},
    {ABILITY_TORRENT.name, &ABILITY_TORRENT},
    {ABILITY_TRUANT.name, &ABILITY_TRUANT},
    {ABILITY_BLAZE.name, &ABILITY_BLAZE},
    {ABILITY_OVERGROW.name, &ABILITY_OVERGROW}
};

std::unordered_map<int16_t, const Ability*> idToAbility;

void mapIDToAbility(int16_t id, const Ability* ability){
    assert(!idToAbility.contains(id));
    assert(id == ability->id);
    idToAbility[id] = ability;
}

void mapIDsToAbilities(){
    assert(abilities.size() > 0);
    for (const auto& [str, ab] : abilities){
        mapIDToAbility(ab->id, ab);
    }
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