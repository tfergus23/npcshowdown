#pragma once
#include <variant>

class Pokemon;

struct BideState{
    Pokemon* lastStruck = nullptr;
    int accumulatedDamage = 0;
    int turns = 0;
};

struct TruantState{
    bool isTruant = false;
};

struct SubstituteState{
    int health = 0;
};

struct SleepState{
    int remainingTurns = 0;
};

struct BadPoisonState{
    int activeTurns = 0;
};

struct ConfusedState{
    int activeTurns = 0;
};

struct ProtectState{
    int protectsInARow = 0;
};

using ObserverState = std::variant<
    BideState,
    TruantState,
    SubstituteState,
    SleepState,
    BadPoisonState,
    ConfusedState,
    ProtectState
>;