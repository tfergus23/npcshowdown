#include "EffectState.hpp"


EffectState::EffectState(){
    reset();
}

void EffectState::reset(){
    remainingTurns = 0;
    suppressed = false;
}