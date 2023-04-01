#pragma once

enum Type{Normal,Fire,Water,Electric,Grass,Ice,Fighting,Poison,Ground,Flying,Psychic,Bug,Rock,Ghost,Dragon,Dark,Steel,None,Fairy};

float typeMatchup(Type attackingType, Type defendingType1, Type defendingType2);

const float ULTRA_EFFECTIVE = 4.0f;
const float SUPER_EFFECTIVE = 2.0f;
const float NORMAL_EFFECTIVE = 1.0f;
const float NOT_VERY_EFFECTIVE = 0.5f;
const float BARELY_EFFECTIVE = 0.25f;