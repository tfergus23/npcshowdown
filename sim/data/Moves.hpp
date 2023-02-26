#pragma once
#include "battle/Move.hpp"

//TODO: implement these
inline const Move MOVE_SWITCH;
inline const Move MOVE_STRUGGLE;

const Move* getMove(const std::string& moveName);
