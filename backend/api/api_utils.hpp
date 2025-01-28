#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include "sim/battle/Battle.hpp"
#include "tflib/config.h"

using json = nlohmann::json;

std::string validateBattleRequest(const json& json);
std::string validatePokemonJSON(const json& json,const std::string& trainerNumber, const std::string& pokemonNumber);
std::string validateTrainerJSON(const json& json, const std::string& trainerNumber);
std::string validateTournamentRequest(const json& json);
std::string validateSaveTournamentRequest(const json& json);
std::string validateAuthRequestSchema(const json& json);
size_t seedFromString(const std::string& seedString);
std::string generateUUID();
std::string sha256(const std::string& str);
int getIntFromConfig(tflib::ini_file& config, const std::string& key);