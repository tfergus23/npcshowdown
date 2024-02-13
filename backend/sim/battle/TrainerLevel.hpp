#pragma once
#include <string>

enum TrainerLevel{FIRST_MOVE, USE_2_MOVES_THEN_SWITCH, SWITCHER, WILD, TRAINER, BOSS};

TrainerLevel trainerLevelFromString(const std::string& string);
std::string stringFromTrainerLevel(TrainerLevel level);