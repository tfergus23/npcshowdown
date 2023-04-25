#pragma once
#include "sim/battle/Move.hpp"

class MoveUse{
public:
    Battle* battle;
    Pokemon* user;
    Pokemon* target;
    bool canCrit = true;
    bool canKill = true;
    bool willFail = false;
    bool wontStart = false;
    std::string failMessage = "But it failed!";
    float damageMod = 1.0f;
    int ppUsage = 1;
    bool usesPP = true;
    bool canDealDamage = true;
    bool canApplyStatus = true;
    bool canLowerStats = true;
    bool canRaiseStats = true;
    bool loggedFailure = false;
    int damageDone = 0;
    bool changeLastMoveUsed = true;
    const Move* move;
    bool logUsed = true;
    int effectivePower = 0;

    //Flags
    bool isSelfDestruct = false;
    bool cantSelfDestruct = false;
    bool sun = false;

    MoveUse();
    MoveUse(const Move* move, Pokemon* user, Pokemon* target, Battle* battle);
    void doMove(MoveUse* opponentMove);
    void fail(const std::string& message);
    void dontStart(const std::string& message);
    void failOnDirectDamage(const std::string& message);
    void failOnSemiInvulnerable(const std::string& message);
private:
    float m_EffectiveAccuracy;
};

bool compareMoves(const MoveUse* move1, const MoveUse* move2);