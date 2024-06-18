#include "sim/battle/MoveUse.hpp"
#include "Stat.hpp"
#include "sim/battle/Battle.hpp"
#include "sim/utils/move_functions.hpp"
#include "sim/utils/stage_multipliers.hpp"
#include "sim/data/Moves.hpp"


MoveUse::MoveUse(){}
MoveUse::MoveUse(const Move* move, Pokemon* user, Pokemon* target, Battle* battle) : move{move}, user{user}, target{target}, battle{battle}, m_EffectiveAccuracy{(float) move->accuracy}, effectivePower{move->power}{
    if (move->targetType == TargetType::SELF) this->target = user;
    m_FailMessage[39] = 0;
}
void MoveUse::doMove(MoveUse* opponentMove){
    //Switching is a special case, since it's not really a 'move'
    if(this->move == &MOVE_SWITCH){
        const TrainerInfo* trainer = battle->player1ActivePokemon == user ? battle->getPlayer1() : battle->getPlayer2();
        battle->log(trainer->name + " withdrew " + user->nickname + "!");
        return;
    }
    battle->raiseEvent(Event::BEFORE_MOVE, EventArgs(nullptr, this));
    move->beforeChecks(this, opponentMove);
    if (wontStart){
        battle->log(m_FailMessage);
        return;
    }
    if (logUsed) battle->log(user->nickname + " used " + move->name + "!");
    if (willFail){
        battle->log(m_FailMessage);
        return;
    }
    if (target->isDead){
        battle->log(m_FailMessage);
        return;
    }
    if (isSelfDestruct){
        selfDestruct(this);
    }
    float typeMod = typeMatchup(move->type, target->currentType[0], target->currentType[1]);
    if (typeMod == NOT_EFFECTIVE && move->damageCategory != DamageCategory::STATUS && move->targetType == TargetType::OPPONENT){
        battle->log("It doesn't affect " + target->nickname + "...");
        if (move->crashOnFail){
            crash(user, battle);
        }
        return;
    }
    int accuracyStage = user->boosts[(int)Stat::ACCURACY] - target->boosts[(int)Stat::EVASION];
    if (accuracyStage > 6) accuracyStage = 6;
    if (accuracyStage < -6) accuracyStage = -6;
    float accuracyMultiplier = accuracyStageMultiplier(accuracyStage);
    m_EffectiveAccuracy = move->accuracy * accuracyMultiplier * user->getCurrentAbility()->accuracyMultiplier;
    battle->debug("Accuracy: " + std::to_string(m_EffectiveAccuracy));
    if (move->accuracy != 0 && m_EffectiveAccuracy < (float) battle->randInt(1,101) && move->targetType == TargetType::OPPONENT){
        battle->log(user->nickname + "'s attack missed!");
        if (move->crashOnFail){
            crash(user,battle);
        }
        return;
    }
    move->afterChecks(this, opponentMove);
}
void MoveUse::fail(std::string_view message){
    willFail = true;
    setFailMessage(message);
}
void MoveUse::dontStart(std::string_view message){
    wontStart = true;
    setFailMessage(message);
    usesPP = false;
}
void MoveUse::failOnDirectDamage(std::string_view message){
    canDealDamage = false;
    setFailMessage(message);
}
void MoveUse::failOnSemiInvulnerable(std::string_view message){
    canApplyStatus = false;
    canDealDamage = false;
    setFailMessage(message);
}

char* MoveUse::getFailMessage(){
    return m_FailMessage;
}
void MoveUse::setFailMessage(std::string_view newMessage){
    strncpy(m_FailMessage, newMessage.data(), 39);
}




bool compareMoves(const MoveUse* move1, const MoveUse* move2){
    //TODO handle pursuit
    if (move1->move->priority != move2->move->priority) return move1->move->priority > move2->move->priority;
    else{
        int move1Speed = move1->user->getStat(Stat::SPEED);
        int move2Speed = move2->user->getStat(Stat::SPEED);
        if (move1Speed != move2Speed) return move1Speed > move2Speed;
        else{
            return move1->battle->randInt(0,2) == 0 ? true : false;
        }
    }
}