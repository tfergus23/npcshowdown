#pragma once
#include "sim/battle/Ability.hpp"
#include <cmath>
#include "sim/data/Statuses.hpp"
#include "sim/battle/MoveUse.hpp"
#include "sim/battle/Battle.hpp"

inline const std::string ABILITY_NONE_NAME = "";
inline const Ability* ABILITY_NONE = nullptr;



inline const Ability ABILITY_GUTS = {
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

inline const Ability ABILITY_TORRENT = {
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
inline const std::unordered_map<std::string,const Ability*> abilities = {
    {ABILITY_NONE_NAME, ABILITY_NONE},
    {ABILITY_GUTS.name, &ABILITY_GUTS},
    {ABILITY_TORRENT.name, &ABILITY_TORRENT}
};