#include "sim/battle/MoveUse.hpp"
#include "Stat.hpp"
#include "sim/battle/Battle.hpp"
#include "sim/utils/move_functions.hpp"
#include "sim/utils/stage_multipliers.hpp"


MoveUse::MoveUse(){}
MoveUse::MoveUse(const Move* move, Pokemon* user, Pokemon* target, Battle* battle) : move{move}, user{user}, target{target}, battle{battle}, m_EffectiveAccuracy{move->accuracy}, effectivePower{move->power}{
    if (move->targetType == SELF) this->target = user;
}
void MoveUse::doMove(MoveUse* opponentMove){
    battle->raiseEvent(BEFORE_MOVE, EventArgs(nullptr, this));
    move->beforeChecks(this, opponentMove);
    if (wontStart){
        battle->log(failMessage);
        return;
    }
    if (logUsed) battle->log(user->nickname + " used " + move->name + "!");
    if (willFail){
        battle->log(failMessage);
        return;
    }
    if (target->isDead){
        battle->log(failMessage);
        return;
    }
    if (isSelfDestruct){
        selfDestruct(this);
    }
    float typeMod = typeMatchup(move->type, target->currentType[0], target->currentType[1]);
    if (typeMod == NOT_EFFECTIVE && move->damageCategory != STATUS && move->targetType == OPPONENT){
        battle->log("It doesn't affect " + target->nickname + "...");
        if (move->crashOnFail){
            crash(user, battle);
        }
        return;
    }
    int accuracyStage = user->boosts[ACCURACY] - target->boosts[EVASION];
    if (accuracyStage > 6) accuracyStage = 6;
    if (accuracyStage < -6) accuracyStage = -6;
    float accuracyMultiplier = accuracyStageMultiplier(accuracyMultiplier);
    m_EffectiveAccuracy = move->accuracy * accuracyMultiplier * user->getCurrentAbility()->accuracyMultiplier;
    battle->debug("Accuracy: " + std::to_string(m_EffectiveAccuracy));
    if (move->accuracy != 0 && m_EffectiveAccuracy < (float) battle->randInt(1,101) && move->targetType == OPPONENT){
        battle->log(user->nickname + "'s attack missed!");
        if (move->crashOnFail){
            crash(user,battle);
        }
        return;
    }
    move->afterChecks(this, opponentMove);
}
void MoveUse::fail(const std::string& message){
    willFail = true;
    failMessage = message;
}
void MoveUse::dontStart(const std::string& message){
    wontStart = true;
    failMessage = message;
    usesPP = false;
}
void MoveUse::failOnDirectDamage(const std::string& message){
    canDealDamage = false;
    failMessage = message;
}
void MoveUse::failOnSemiInvulnerable(const std::string& message){
    canApplyStatus = false;
    canDealDamage = false;
    failMessage = message;
}






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