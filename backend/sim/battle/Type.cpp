#include "sim/battle/Type.hpp"

static_assert(typeMatchup(Type::FIRE, Type::WATER, Type::NONE) == NOT_VERY_EFFECTIVE);
static_assert(typeMatchup(Type::FIRE, Type::GRASS, Type::NONE) == SUPER_EFFECTIVE);
static_assert(typeMatchup(Type::GRASS, Type::WATER, Type::GROUND) == ULTRA_EFFECTIVE);
static_assert(typeMatchup(Type::NORMAL, Type::GHOST, Type::STEEL) == NOT_EFFECTIVE);
static_assert(typeMatchup(Type::ELECTRIC, Type::GHOST, Type::STEEL) == NORMAL_EFFECTIVE);
static_assert(typeMatchup(Type::NONE, Type::GHOST, Type::NONE) == NORMAL_EFFECTIVE);
static_assert(typeMatchup(Type::NONE, Type::NONE, Type::NONE) == NORMAL_EFFECTIVE);
static_assert(typeMatchup(Type::FIGHTING, Type::NORMAL, Type::GHOST) == NOT_EFFECTIVE);
static_assert(typeMatchup(Type::FIGHTING, Type::NORMAL, Type::NONE) == SUPER_EFFECTIVE);
static_assert(typeMatchup(Type::ELECTRIC, Type::WATER, Type::FLYING) == ULTRA_EFFECTIVE);
static_assert(typeMatchup(Type::NORMAL, Type::ROCK, Type::STEEL) == BARELY_EFFECTIVE);
static_assert(typeMatchup(Type::DARK, Type::STEEL, Type::NONE) == NORMAL_EFFECTIVE);
static_assert(typeMatchup(Type::GHOST, Type::STEEL, Type::NONE) == NORMAL_EFFECTIVE);