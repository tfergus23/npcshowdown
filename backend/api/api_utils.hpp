#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include "sim/battle/Battle.hpp"

using json = nlohmann::json;

std::string validateBattleRequest(const json& json);
std::string validatePokemonJSON(const json& json,const std::string& trainerNumber, const std::string& pokemonNumber);
std::string validateTrainerJSON(const json& json, const std::string& trainerNumber);
std::string validateTournamentRequest(const json& json);
Battle battleFromRequest(const json& json);
size_t seedFromString(const std::string& seedString);