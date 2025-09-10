#pragma once
#include "sim/battle/Pokemon.hpp"
#include "sim/battle/Move.hpp"
#include <string>


class MoveUse{
public:
    Battle* battle;
    Pokemon* user;
    Pokemon* target;
    bool canCrit = true;
    bool canKill = true;
    bool willFail = false;
    bool wontStart = false;
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
    Type effectiveType = Type::NONE;

    //Flags
    bool isSelfDestruct = false;
    bool cantSelfDestruct = false;
    bool sun = false;
    bool guts = false;

    MoveUse();
    MoveUse(const Move* move, Pokemon* user, Pokemon* target, Battle* battle);
    void doMove(MoveUse* opponentMove);
    void fail(std::string_view message);
    void dontStart(std::string_view message);
    void failOnDirectDamage(std::string_view message);
    void failOnSemiInvulnerable(std::string_view message);
    char* getFailMessage();
    void setFailMessage(std::string_view newMessage);
    // Logs as an attack if the move did damage, logs a basic message otherwise
    void logUsage();
private:
    static constexpr int FAIL_MESSAGE_LENGTH = 40;
    float m_EffectiveAccuracy;
    char m_FailMessage[FAIL_MESSAGE_LENGTH] = "But it failed!";
};

bool compareMoves(const MoveUse* move1, const MoveUse* move2);