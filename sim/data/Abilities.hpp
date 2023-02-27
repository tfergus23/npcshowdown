#pragma once
#include "battle/Ability.hpp"
#include <cmath>
#include "data/Statuses.hpp"

inline const std::string ABILITY_NONE_NAME = "";
inline const Ability* ABILITY_NONE = nullptr;


//Guts
inline const std::string ABILITY_GUTS_NAME = "Guts";
class Guts: public Ability{
public:
    Guts(){
        name = ABILITY_GUTS_NAME;
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
inline const std::string ABILITY_TORRENT_NAME = "Torrent";
class Torrent: public Ability{
public:
    Torrent(){
        name = ABILITY_TORRENT_NAME;
    }

    void beforeMove(MoveUse* moveUse,Pokemon* subject) const {
        //TODO
    }
};
inline const Torrent ABILITY_TORRENT;





//Mapping string of name to ability
std::unordered_map<std::string,const Ability*> abilities = {
    {ABILITY_NONE_NAME, ABILITY_NONE},
    {ABILITY_GUTS_NAME, &ABILITY_GUTS},
    {ABILITY_TORRENT_NAME, &ABILITY_TORRENT}
};