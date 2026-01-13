#pragma once
#include <variant>
#include "sim/data/ObserverStates.hpp"

using ObserverState = std::variant<
    BideState,
    TruantState,
    SubstituteState,
    SleepState,
    BadPoisonState,
    ConfusedState,
    ProtectState,
    ScreenState
>;