#pragma once
#include <string>
#include <unordered_map>

enum Nature {HARDY,LONELY,BRAVE,ADAMANT,NAUGHTY,BOLD,DOCILE,RELAXED,IMPISH,LAX,TIMID,HASTY,SERIOUS,JOLLY,NAIVE,MODEST,MILD,QUIET,BASHFUL,RASH,CALM,GENTLE,SASSY,CAREFUL,QUIRKY};

inline const std::unordered_map<std::string, Nature> natures = {
    {"Hardy", HARDY},
    {"Lonely", LONELY},
    {"Brave", BRAVE},
    {"Adamant", ADAMANT},
    {"Naughty", NAUGHTY},
    {"Bold", BOLD},
    {"Docile", DOCILE},
    {"Relaxed", RELAXED},
    {"Impish", IMPISH},
    {"Lax", LAX},
    {"Timid", TIMID},
    {"Hasty", HASTY},
    {"Serious", SERIOUS},
    {"Jolly", JOLLY},
    {"Modest", MODEST},
    {"Mild", MILD},
    {"Quiet", QUIET},
    {"Bashful", BASHFUL},
    {"Rash", RASH},
    {"Calm", CALM},
    {"Gentle", GENTLE},
    {"Sassy", SASSY},
    {"Careful", CAREFUL},
    {"Quirky", QUIRKY}
};

float natureChart[25][5] = {
    {1.0f,1.0f,1.0f,1.0f,1.0f},
    {1.1f,0.9f,1.0f,1.0f,1.0f},
    {1.1f,1.0f,1.0f,1.0f,0.9f},
    {1.1f,1.0f,0.9f,1.0f,1.0f},
    {1.1f,1.0f,1.0f,0.9f,1.0f},
    {0.9f,1.1f,1.0f,1.0f,1.0f},
    {1.0f,1.0f,1.0f,1.0f,1.0f},
    {1.0f,1.1f,1.0f,1.0f,0.9f},
    {1.0f,1.1f,0.9f,1.0f,1.0f},
    {1.0f,1.1f,1.0f,0.9f,1.0f},
    {0.9f,1.0f,1.0f,1.0f,1.1f},
    {1.0f,0.9f,1.0f,1.0f,1.1f},
    {1.0f,1.0f,1.0f,1.0f,1.0f},
    {1.0f,1.0f,0.9f,1.0f,1.1f},
    {1.0f,1.0f,1.0f,0.9f,1.1f},
    {0.9f,1.0f,1.1f,1.0f,1.0f},
    {1.0f,0.9f,1.1f,1.0f,1.0f},
    {1.0f,1.0f,1.1f,1.0f,0.9f},
    {1.0f,1.0f,1.0f,1.0f,1.0f},
    {1.0f,1.0f,1.1f,0.9f,1.0f},
    {0.9f,1.0f,1.0f,1.1f,1.0f},
    {1.0f,0.9f,1.0f,1.1f,1.0f},
    {1.0f,1.0f,1.0f,1.1f,0.9f},
    {1.0f,1.0f,0.9f,1.1f,1.0f},
    {1.0f,1.0f,1.0f,1.0f,1.0f}
};

float natureBoost(Nature nature, Stat stat);