#pragma once

struct EffectState{
    //Any time you add a new var, initialize it in the reset function
    int remainingTurns;
    bool suppressed;

    EffectState();
    void reset();
};