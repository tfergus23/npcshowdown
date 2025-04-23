#pragma once

class Pokemon;
/*
struct EffectState{
    //Any time you add a new var, initialize it in the reset function
    int remainingTurns;
    bool suppressed;
    int substituteHealth;
    bool isTruant;

    EffectState();
    void reset();
};
*/

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

union ObserverState{
    BideState bideState;
    TruantState truantState;
    SubstituteState substituteState;
    ObserverState(){}
};