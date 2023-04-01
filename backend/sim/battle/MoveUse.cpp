#include "sim/battle/MoveUse.hpp"
#include "Stat.hpp"

bool compareMoves(const MoveUse* move1, const MoveUse* move2){
    //TODO handle pursuit
    if (move1->move->priority != move2->move->priority) return move1->move->priority > move2->move->priority;
    else{
        int move1Speed = move1->user->getStat(SPEED);
        int move2Speed = move2->user->getStat(SPEED);
        if (move1Speed != move2Speed) return move1Speed > move2Speed;
        else{
            return move1->battle->randInt(0,2) == 0 ? true : false;
        }
    }
}