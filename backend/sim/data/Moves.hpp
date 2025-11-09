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
extern const Move MOVE_TOXIC;
extern const Move MOVE_SPORE;
extern const Move MOVE_THUNDER_WAVE;
extern const Move MOVE_WILL_O_WISP;
extern const Move MOVE_POISON_POWDER;
extern const Move MOVE_POISON_GAS;
extern const Move MOVE_PROTECT;
extern const Move MOVE_LEECH_SEED;
extern const Move MOVE_FLY;
extern const Move MOVE_REFLECT;

extern const Move* moveFromString(const std::string& moveName);
std::string createMoveDataResponse();
void mapIDsToMoves();
const Move* moveFromID(int16_t id);