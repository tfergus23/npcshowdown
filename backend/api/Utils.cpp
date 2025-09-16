#include "./Utils.hpp"
#include <vector>
#include "sim/battle/Battle.hpp"
#include "sim/data/Moves.hpp"
#include "sim/data/Abilities.hpp"
#include <iostream>
#include <climits>
#include "sim/tournament/Tournament.hpp"
#include <sstream>
#include <iomanip>
#include <fstream>
#include <openssl/evp.h>
#include "tflib/strings.h"
#ifdef _WIN32
#include <windows.h>
#include <wincrypt.h>
#endif

constexpr int MAX_TRAINER_NAME_SIZE     =    16;
constexpr int MAX_POKEMON_NAME_SIZE     =    16;
constexpr int MAX_TOURNAMENT_NAME_SIZE  =    32;
constexpr int MAX_USER_NAME_SIZE        =    16;
constexpr int MIN_PASSWORD_LENGTH       =     8;
constexpr int MAX_POKEMON_LEVEL         =  1000;

static const std::hash<std::string> hasher;
static const std::string API_AUTH_ENDPOINT = "http://npcshowdown.com/api/auth";

std::string checkForString(const json& json, const std::string& prefix, const std::string& fieldName){
    std::string problems = "";
    if (!json.contains(fieldName)){
        problems += "Bad Request: Undefined required field '" + prefix + fieldName + "'\n";
    }
    else if (!json[fieldName].is_string()){
        problems += "Bad Request: '" + prefix + fieldName + "' must be of type string, was " + std::string(json[fieldName].type_name()) + "\n";
    }
    return problems;
}

std::string checkForInt(const json& json, const std::string& prefix, const std::string& fieldName){
    std::string problems = "";
    if (!json.contains(fieldName)){
        problems += "Bad Request: Undefined required field '" + prefix + fieldName + "'\n";
    }
    else if (!json[fieldName].is_number_integer()){
        problems += "Bad Request: '" + prefix + fieldName + "' must be of type number (integer), was " + std::string(json[fieldName].type_name()) + "\n";
    }
    return problems;
}

std::string checkForObject(const json& json, const std::string& prefix, const std::string& fieldName){
    std::string problems = "";
    if (!json.contains(fieldName)){
        problems += "Bad Request: Undefined required field '" + prefix + fieldName + "'\n";
    }
    else if (!json[fieldName].is_object()){
        problems += "Bad Request: '" + prefix + fieldName + "' must be of type object, was " + std::string(json[fieldName].type_name()) + "\n";
    }
    return problems;
}

std::string checkForFixedIntArray(const json& json, const std::string& prefix, const std::string& fieldName, size_t arraySize){
    std::string problems = "";
    if (!json.contains(fieldName)){
        problems += "Bad Request: Undefined required field '" + prefix + fieldName + "'\n";
    }
    else if (!json[fieldName].is_array()){
        problems += "Bad Request: '" + prefix + fieldName + "' must be an array of exactly " + std::to_string(arraySize) + " integers, was " + std::string(json[fieldName].type_name()) + "\n";
    }
    else if (json[fieldName].size() != arraySize){
        problems += "Bad Request: '" + prefix + fieldName + "' must be an array of exactly " + std::to_string(arraySize) + " integers, had " + std::to_string(json[fieldName].size()) + " elements\n";
    }
    else{
        for(int i = 0; i < json[fieldName].size(); i++){
            std::string arrayFieldName = prefix + fieldName + "[" + std::to_string(i) + "]";
            if (!json[fieldName][i].is_number_integer()){
                problems += "Bad Request: " + arrayFieldName + " must be of type number (integer), was " + std::string(json[fieldName][i].type_name()) + "\n";
            }
        }
    }
    return problems;
}

std::string checkForFixedStringArray(const json& json, const std::string& prefix, const std::string& fieldName, size_t arraySize){
    std::string problems = "";
    if (!json.contains(fieldName)){
        problems += "Bad Request: Undefined required field '" + prefix + fieldName + "'\n";
    }
    else if (!json[fieldName].is_array()){
        problems += "Bad Request: '" + prefix + fieldName + "' must be an array of exactly " + std::to_string(arraySize) + " strings, was " + std::string(json[fieldName].type_name()) + "\n";
    }
    else if (json[fieldName].size() != arraySize){
        problems += "Bad Request: '" + prefix + fieldName + "' must be an array of exactly " + std::to_string(arraySize) + " strings, had " + std::to_string(json[fieldName].size()) + " elements\n";
    }
    else{
        for(int i = 0; i < json[fieldName].size(); i++){
            std::string arrayFieldName = prefix + fieldName + "[" + std::to_string(i) + "]";
            if (!json[fieldName][i].is_string()){
                problems += "Bad Request: " + arrayFieldName + " must be of type string, was " + std::string(json[fieldName][i].type_name()) + "\n";
            }
        }
    }
    return problems;
}

std::string checkForDynamicObjectArray(const json& json, const std::string& prefix, const std::string& fieldName, size_t maxArraySize){
    std::string problems = "";
    if (!json.contains(fieldName)){
        problems += "Bad Request: Undefined required field '" + prefix + fieldName + "'\n";
    }
    else if (!json[fieldName].is_array()){
        problems += "Bad Request: '" + prefix + fieldName + "' must be an array of no more than " + std::to_string(maxArraySize) + " objects, was " + std::string(json[fieldName].type_name()) + "\n";
    }
    else if (json[fieldName].size() > maxArraySize){
        problems += "Bad Request: '" + prefix + fieldName + "' must be an array of no more than " + std::to_string(maxArraySize) + " objects, had " + std::to_string(json[fieldName].size()) + " elements\n";
    }
    else{
        for(int i = 0; i < json[fieldName].size(); i++){
            std::string arrayFieldName = prefix + fieldName + "[" + std::to_string(i) + "]";
            if (!json[fieldName][i].is_object()){
                problems += "Bad Request: " + arrayFieldName + " must be of type object, was " + std::string(json[fieldName][i].type_name()) + "\n";
            }
        }
    }
    return problems;
}

std::string validateBattleRequest(const json& json){
    std::string problems = "";

    //Validate schema
    problems += checkForObject(json, "", "trainer1");
    problems += checkForObject(json, "", "trainer2");
    problems += checkForString(json, "", "seed");
    problems += checkForString(json, "", "type");

    if (problems != ""){
        return problems;
    }

    std::string type = json["type"].get<std::string>();

    if (type != "text" && type != "events"){
        problems += "Invalid battle type: '" + type + "'. Only 'text' and 'events' allowed.";
    }

    problems += validateTrainerJSON(json["trainer1"], "1");
    problems += validateTrainerJSON(json["trainer2"], "2");

    return problems;

}

std::string validateTrainerJSON(const json& json, const std::string& trainerNumber){
    std::string problems = "";
    std::string trainerPrefix = "trainer" + trainerNumber + ".";
    std::string trainerFriendlyName = "Trainer";
    if (trainerNumber !=  ""){
        trainerFriendlyName += " ";
    }
    trainerFriendlyName += trainerNumber;

    //Validate schema
    problems += checkForString(json, trainerPrefix, "name");
    problems += checkForString(json, trainerPrefix, "trainerLevel");
    problems += checkForDynamicObjectArray(json, trainerPrefix, "team", 6);

    if (problems != ""){
        return problems;
    }

    //Validate user errors
    if (json["name"].get<std::string>().size() > MAX_TRAINER_NAME_SIZE){
        problems += trainerFriendlyName + "'s name is too long, max " + std::to_string(MAX_TRAINER_NAME_SIZE) + " characters.\n";
    }
    else if(json["name"].get<std::string>() == ""){
        problems += trainerFriendlyName + " needs a name.\n";
    }
    std::string trainerLevel = json["trainerLevel"].get<std::string>();
    try{
        trainerLevelFromString(trainerLevel);
    }
    catch(...){
        problems += "Invalid trainer level: " + trainerLevel + "\n";
    }
    if (json["team"].size() == 0){
        problems += trainerFriendlyName + " has no pokemon.\n";
    }

    //Validate all the pokemon
    for(int i = 0; i < json["team"].size(); i++){
        problems += validatePokemonJSON(json["team"][i], trainerNumber, std::to_string(i));
    }
    return problems;
}

std::string validatePokemonJSON(const json& json,const std::string& trainerNumber, const std::string& pokemonNumber){
    std::string pokemonPrefix = "trainer" + trainerNumber  + ".team[" + pokemonNumber + "].";
    std::string pokemonFriendlyName = "Trainer " + trainerNumber + ", Pokemon " + std::to_string(stoi(pokemonNumber)+1);
    std::string problems = "";

    //Validate schema
    problems += checkForString(json, pokemonPrefix, "species");
    problems += checkForInt(json, pokemonPrefix, "level");
    problems += checkForFixedStringArray(json, pokemonPrefix, "moves", 4);
    problems += checkForString(json, pokemonPrefix, "abilityName");
    problems += checkForString(json, pokemonPrefix, "gender");
    problems += checkForFixedIntArray(json, pokemonPrefix, "ivs", 6);
    problems += checkForString(json, pokemonPrefix, "nature");
    problems += checkForString(json, pokemonPrefix, "itemName");
    problems += checkForFixedIntArray(json, pokemonPrefix, "evs", 6);
    problems += checkForString(json, pokemonPrefix, "nickname");

    if (problems != ""){
        return problems;
    }

    //Validate user errors
    try{
        speciesFromString(json["species"].get<std::string>());
    }
    catch(...){
        if (json["species"].get<std::string>() == ""){
            problems += pokemonFriendlyName + " needs a species.\n";
        }
        else{
            problems += pokemonFriendlyName + " has an invalid species: '" + json["species"].get<std::string>() + "'\n";
        }
    }
    if (json["level"].get<int>() < 1 || json["level"].get<int>() > MAX_POKEMON_LEVEL){
        problems += pokemonFriendlyName + " has an invalid level.\n";
    }
    bool foundAMove = false;
    for(int i = 0; i < json["moves"].size(); i++){
        std::string friendlyName = pokemonFriendlyName + ", move " + std::to_string(i+1);
        try{
            const Move* move = moveFromString(json["moves"][i].get<std::string>());
            if (move != &MOVE_NONE){
                foundAMove = true;
            }
        }
        catch (...){
            problems += friendlyName + " is invalid or unimplemented.\n";
            foundAMove = true;
        }
    }
    if (!foundAMove){
        problems += pokemonFriendlyName + " has no moves.\n";
    }
    try {
        abilityFromString(json["abilityName"].get<std::string>());
    }
    catch(...){
        problems += pokemonFriendlyName + "'s ability is invalid or unimplemented.\n";
    }
    std::string genderString = json["gender"].get<std::string>();
    if (genderString != "Random" && genderString != "Male" && genderString != "Female"){
        problems += pokemonFriendlyName + " has an invalid gender.\n";
    }

    auto ivs = json["ivs"].get<std::array<int,6>>();
    for(auto iv : ivs){
        if (iv < 0 || iv > 31){
            problems += pokemonFriendlyName + " has invalid IVs.\n";
            break;
        }
    }

    try{
        natureFromString(json["nature"].get<std::string>());
    }
    catch(...){
        problems += pokemonFriendlyName + " has an invalid nature.\n";
    }

    try {
        itemFromString(json["itemName"].get<std::string>());
    }
    catch (...){
        problems += pokemonFriendlyName + "'s item is invalid or unimplemented.\n";
    }
    auto evs = json["evs"].get<std::array<int,6>>();
    for(auto ev : evs){
        if (ev < 0 || ev > 255){
            problems += pokemonFriendlyName + " has invalid EVs.\n";
            break;
        }
    }
    if (json["nickname"].get<std::string>().size() > MAX_POKEMON_NAME_SIZE){
        problems += pokemonFriendlyName + "'s nickname is too long, max " + std::to_string(MAX_POKEMON_NAME_SIZE) + " characters.\n";
    }
    return problems;
}

std::string validateTournamentRequest(const json& json){
    std::string problems = "";

    //Validate Schema
    problems += checkForDynamicObjectArray(json, "", "trainers", INT_MAX);
    problems += checkForString(json, "", "seed");
    problems += checkForInt(json, "", "rounds");

    //user is optional
    if (json.contains("user") && !json["user"].is_string()){
        problems += "Bad Request: 'user' must be of type string, was " + std::string(json["user"].type_name()) + "\n";
    }

    //Name is optional
    if (json.contains("name") && !json["name"].is_string()){
        problems += "Bad Request: 'name' must be of type string, was " + std::string(json["name"].type_name()) + "\n";
    }

    if (problems != ""){
        return problems;
    }

    if (json["trainers"].size() < 2){
        return "Need to have at least 2 trainers to have a tournament, " + std::to_string(json["trainers"].size()) + " supplied.\n";
    }

    
    if (json["rounds"].get<int>() < 1){
        return "Can't have a tournament with zero or fewer rounds.\n";
    }

    if (json.contains("name")){
        std::string name = tflib::trim(json["name"].get<std::string>());
        if (name.size() > MAX_TOURNAMENT_NAME_SIZE){
            return "Tournament names cannot be longer than " + std::to_string(MAX_TOURNAMENT_NAME_SIZE) + " characters. (" + std::to_string(name.size()) + " supplied)";
        }
    }
    

    if (calculateTotalBattles(json["trainers"].size(), json["rounds"].get<int>()) > MAX_TOURNAMENT_BATTLES){
        return "Too many battles in requested tournament, " + std::to_string(MAX_TOURNAMENT_BATTLES) + " max.\n";
    }

    for(size_t i = 0; i < json["trainers"].size(); i++){
        problems += validateTrainerJSON(json["trainers"][i], std::to_string(i+1));
    }

    return problems;
}

std::string validateAuthRequestSchema(const json& json){
    std::string problems = "";

    problems += checkForString(json, "", "username");
    problems += checkForString(json, "", "password");

    return problems;
}

size_t seedFromString(const std::string& seedString){
    size_t seed = 0;
    try {
        seed = stoul(seedString);
    }
    catch(...){
        seed = hasher(seedString);
    }
    return seed;
}

std::string charArrayToHex(const char* arr, int len) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (int i = 0; i < len; ++i) {
        ss << std::setw(2) << static_cast<unsigned int>(static_cast<unsigned char>(arr[i]));
    }
    return ss.str();
}

std::string generateUUID(){
#ifdef __linux__
    std::ifstream random("/dev/random", std::ios::binary);

    if (!random) {
        throw std::runtime_error("Error opening /dev/random");
    }

    std::vector<char> buffer(16);
    random.read(buffer.data(), buffer.size());

    if (!random) {
        throw std::runtime_error("Error opening /dev/random");
    }

    return charArrayToHex(buffer.data(), buffer.size());
#endif
#ifdef _WIN32
    std::vector<BYTE> buffer(16);

    HCRYPTPROV hProv = 0;
    if (!CryptAcquireContext(&hProv, nullptr, nullptr, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
        throw std::runtime_error("CryptAcquireContext failed");
    }

    if (!CryptGenRandom(hProv, static_cast<DWORD>(16), buffer.data())) {
        CryptReleaseContext(hProv, 0);
        throw std::runtime_error("CryptGenRandom failed");
    }

    CryptReleaseContext(hProv, 0);

    return charArrayToHex((char*)buffer.data(), buffer.size());
#endif
}

std::string sha256(const std::string &input) {
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len = 0;

    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    if (!ctx) {
        throw std::runtime_error("Error: Could not create EVP_MD_CTX");
    }

    if (EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr) != 1) {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("Error: EVP_DigestInit_ex failed");
    }

    if (EVP_DigestUpdate(ctx, input.c_str(), input.length()) != 1) {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("Error: EVP_DigestUpdate failed");
    }

    if (EVP_DigestFinal_ex(ctx, hash, &hash_len) != 1) {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("Error: EVP_DigestFinal_ex failed");
    }

    EVP_MD_CTX_free(ctx);

    std::string result;
    for (unsigned int i = 0; i < hash_len; ++i) {
        char buf[3];
        snprintf(buf, 3, "%02x", hash[i]);
        result += buf;
    }

    return result;
}

std::string validateSaveTournamentRequest(const json& json){
    std::string problems = "";

    problems += checkForInt(json, "", "tournamentID");

    return problems;
}

std::string validateEmailUpdateRequest(const json& json){
    std::string problems = "";

    problems += checkForString(json, "", "newEmail");

    if (problems != ""){
        return problems;
    }

    std::string newEmail = json["newEmail"].get<std::string>();

    if (newEmail.size() > 254){
        problems += "Emails cannot be longer than 254 characters.\n";
    }

    if (newEmail.size() == 0){
        problems += "Please enter an email.\n";
    }

    return problems;
}

std::string validateUpdatePasswordRequest(const json& json){
    std::string problems = "";

    problems += checkForString(json, "", "currentPassword");
    problems += checkForString(json, "", "newPassword");

    if (problems != ""){
        return problems;
    }

    if (json["newPassword"].get<std::string>().size() < MIN_PASSWORD_LENGTH){
        problems += "Passwords must be at least " + std::to_string(MIN_PASSWORD_LENGTH) + " characters.\n";
    }

    return problems;
}

std::string validateCreateUserRequest(const json& json){
    std::string problems = "";

    problems += checkForString(json, "", "username");
    problems += checkForString(json, "", "password");

    if (problems != ""){
        return problems;
    }

    std::string username = tflib::trim(json["username"].get<std::string>());
    std::string password = json["password"].get<std::string>();

    if (username.find(' ') != std::string::npos){
        problems += "Usernames cannot have spaces.\n";
    }

    if (username.size() > MAX_USER_NAME_SIZE){
        problems += "Usernames cannot be longer than " + std::to_string(MAX_USER_NAME_SIZE) + " characters.\n";
    }

    if (password.size() < MIN_PASSWORD_LENGTH){
        problems += "Passwords must be at least " + std::to_string(MIN_PASSWORD_LENGTH) + " characters.\n";
    }

    return problems;
}

std::uint64_t fnv1a_hash_64(const std::string& str) {
    const std::uint64_t FNV_OFFSET_BASIS = 14695981039346656037ull;
    const std::uint64_t FNV_PRIME = 1099511628211ull;

    std::uint64_t hash = FNV_OFFSET_BASIS;

    for (char c : str) {
        hash ^= static_cast<std::uint8_t>(c);
        hash *= FNV_PRIME;
    }

    return hash;
}

bool isUnsignedInteger(const std::string& str) {
    try {
        size_t pos;
        size_t num = std::stoul(str, &pos);
        return pos == str.size() && num <= UINT32_MAX; // Ensure the whole string was converted
    } catch (const std::invalid_argument& e) {
        return false; // Not a number at all
    } catch (const std::out_of_range& e) {
        return false; // Number out of int range
    }
}