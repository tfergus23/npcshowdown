#pragma once
#include "sim/battle/Ability.hpp"
#include <cmath>
#include "sim/data/Statuses.hpp"

inline const std::string ABILITY_NONE_NAME = "";
inline const Ability* ABILITY_NONE = nullptr;


//Guts
class Guts: public Ability{
public:
    Guts(){
        name = "Guts";
    }
    int modifySubjectStat(Stat stat,int statVal,Pokemon* subject) const {
        if (stat == ATTACK && subject->getStatus() != STATUS_NONE){
            float newVal = (float) statVal * 1.5f;
            return (int) floor(statVal);
        } 
        return statVal;
    }
};
inline const Guts ABILITY_GUTS;

//Torrent
class Torrent: public Ability{
public:
    Torrent(){
        name = "Torrent";
    }
protected:
    void beforeMove(Pokemon* subject, Battle* battle, const EventArgs& args) const {
        //TODO
        if (args.moveUse->user == subject && (float) args.moveUse->user->currentHealth / (float) args.moveUse->user->getStat(HP, subject->battle) <= (1.0f / 3.0f) && args.moveUse->move->type == WATER){
            args.moveUse->damageMod *= 1.5f;
        }
    }
};
inline const Torrent ABILITY_TORRENT;





//Mapping string of name to ability
inline const std::unordered_map<std::string,const Ability*> abilities = {
    {ABILITY_NONE_NAME, ABILITY_NONE},
    {ABILITY_GUTS.name, &ABILITY_GUTS},
    {ABILITY_TORRENT.name, &ABILITY_TORRENT}
};