#pragma once
#include "sim/battle/Move.hpp"

extern const Move MOVE_NONE;
extern const Move MOVE_SWITCH;
extern const Move MOVE_STRUGGLE;
extern const Move MOVE_POUND;
extern const Move MOVE_TACKLE;
extern const Move MOVE_KARATE_CHOP;
extern const Move MOVE_SURF;
extern const Move MOVE_FLAMETHROWER;
extern const Move MOVE_CONFUSION_HIT;
extern const Move* moveFromString(const std::string& moveName);
std::string createMoveDataResponse();
void mapIDsToMoves();
const Move* moveFromID(int16_t id);