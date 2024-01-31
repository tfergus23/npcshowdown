#pragma once
#include "sim/battle/Move.hpp"

extern const Move* MOVE_NONE;
extern const std::string MOVE_NONE_NAME;
extern const Move MOVE_SWITCH;
extern const Move MOVE_STRUGGLE;
extern const Move MOVE_POUND;
extern const Move MOVE_TACKLE;
extern const Move MOVE_KARATE_CHOP;
extern const Move* moveFromString(const std::string& moveName);