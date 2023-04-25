#pragma once
#include "sim/battle/Status.hpp"

inline const Status* STATUS_NONE = nullptr;

//TODO
class Paralysis : public Status{

};
inline const Paralysis STATUS_PARALYSIS;

//TODO
class Poison: public Status{

};
inline const Poison STATUS_POISON;

//TODO
class Sleep: public Status{

};
inline const Sleep STATUS_SLEEP;

//TODO
class Burn: public Status{

};
inline const Burn STATUS_BURN;

//TODO
class Frozen : public Status{

};
inline const Frozen STATUS_FROZEN;

class BadPoison: public Status{

};
inline const BadPoison STATUS_BAD_POISON;