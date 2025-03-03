#pragma once
#include <stdint.h>

enum class Type : uint8_t {NORMAL,FIRE,WATER,ELECTRIC,GRASS,ICE,FIGHTING,POISON,GROUND,FLYING,PSYCHIC,BUG,ROCK,GHOST,DRAGON,DARK,STEEL,NONE,FAIRY};

float typeMatchup(Type attackingType, Type defendingType1, Type defendingType2);

constexpr float ULTRA_EFFECTIVE = 4.0f;
constexpr float SUPER_EFFECTIVE = 2.0f;
constexpr float NORMAL_EFFECTIVE = 1.0f;
constexpr float NOT_VERY_EFFECTIVE = 0.5f;
constexpr float BARELY_EFFECTIVE = 0.25f;
constexpr float NOT_EFFECTIVE = 0.0f;