#pragma once
#include "battle/Move.hpp"
#include "utils/move_functions.hpp"

inline const Move* MOVE_NONE = nullptr;

//TODO: implement these
inline const Move MOVE_SWITCH;
inline const Move MOVE_STRUGGLE;

const Move* getMove(const std::string& moveName);


inline const std::string MOVE_POUND_NAME = "Pound";
class Pound: public Move{
public:
    Pound(){
        name = MOVE_POUND_NAME;
        type = Normal;
        damageCategory = PHYSICAL;
        power = 40;
        accuracy = 100;
        maxPP = 56;
        priority = 0;
        critRatio = 0;
        targetType = OPPONENT;
        secondaryEffect = NOEFFECT;
        secondaryEffectChance = -1;
        secondaryEffectValue = -1;

        //Flags
        contact = true;
        protect = true;
        mirrorMove = true;
        kingsRock = true;
    }

    void afterChecks(MoveUse* myMove, MoveUse* opponentMove){
        dealDirectDamage(myMove);
    }
};
inline const Pound MOVE_POUND;

inline const std::unordered_map<std::string, const Move*> moves = {
    {MOVE_POUND_NAME, &MOVE_POUND}
};