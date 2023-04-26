#pragma once
#include <unordered_map>
#include <string>

enum Gender{MALE, FEMALE, GENDERLESS};


inline const std::unordered_map<std::string, const Gender> genders = {
    {"", GENDERLESS},
    {"Male", MALE},
    {"Female", FEMALE},
    {"Genderless", GENDERLESS}
};