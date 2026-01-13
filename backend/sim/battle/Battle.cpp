#include "sim/battle/Battle.hpp"
#include <random>
#include "sim/data/Moves.hpp"
#include "sim/data/Weathers.hpp"
#include "sim/data/Items.hpp"
#include <iostream>

Battle::Battle(const Trainer& trainer1, const Trainer& trainer2, size_t seed, const DebugOptions&& debugOptions) :
m_Player1{trainer1.trainerInfo},
m_Player2{trainer2.trainerInfo},
m_Seed{seed},
debugOptions{debugOptions},
m_Generator{std::default_random_engine(m_Seed)}
{
    for (int i = 0; i < trainer1.teamBlueprint.size(); i++){
        player1Team[i] = Pokemon(&trainer1.teamBlueprint[i], this);
    }
    for (int i = 0; i < trainer2.teamBlueprint.size(); i++){
        player2Team[i] = Pokemon(&trainer2.teamBlueprint[i], this);
    }
    setActivePokemon(true, 0);
    setActivePokemon(false, 0);
    setPokemonHandlerOrder();
    raiseEvent(Event::POKEMON_ENTER, EventArgs(m_FasterPokemon, nullptr));
    raiseEvent(Event::POKEMON_ENTER, EventArgs(m_SlowerPokemon, nullptr));
}

void Battle::simulate(){
    try{
        while (!this->isBattleOver){
            const Move* player1Move = this->getPlayer1()->pickMove(this->player1ActivePokemon, this->player2ActivePokemon, this);
            const Move* player2Move = this->getPlayer2()->pickMove(this->player2ActivePokemon, this->player1ActivePokemon, this);
            this->addMoves(player1Move, player2Move);
            while (!this->isTurnOver){
                this->doMove();
                this->switchIfNecessary();
                if (this->moveNumber == 1){
                    this->raiseEvent(Event::END_OF_TURN, EventArgs(nullptr, nullptr));
                    int sanityCheck = 0;
                    // If a pokemon dies as soon as it comes out, we have to switch again. Keep switching until both aren't dead.
                    while (!isBattleOver && (player1ActivePokemon->isDead || player2ActivePokemon->isDead)){
                        this->switchIfNecessary();
                        sanityCheck++;
                        assertTrue(sanityCheck < 7, "Stuck in switchIfNecessary() loop.");
                    }
                }
                else{
                    this->moveNumber++;
                }
            }
        }
    }
    catch (const std::exception& e){
        this->invalid = true;
        this->isTurnOver = true;
        this->isBattleOver = true;
        this->isDraw = true;
        this->winner = nullptr;
        this->errorMessage = e.what();
    }
}

const TrainerInfo* Battle::getPlayer1() const {
    return &m_Player1;
}
const TrainerInfo* Battle::getPlayer2() const {
    return &m_Player2;
}

void Battle::debug(std::string_view message){
    if (debugOptions.debugLogging){
        logMessage(message);
    }
}

void Battle::assertTrue(bool condition, std::string_view message){
    if (!condition){
        logMessage("Assertion failed! Stopping battle.");
        errorMessage = std::string(message);
        throw BattleAssertionFailedException(message);
    }
}



int Battle::randInt(int min, int max){
    std::uniform_int_distribution<int> distribution(min, max-1);
    int dice_roll = distribution(m_Generator);
    return dice_roll;
}

size_t Battle::getSeed(){
    return m_Seed;
}

void Battle::setMoveUse(const Move* intendedMove, Pokemon* user, Pokemon* enemy, const TrainerInfo* trainer){
    bool isPlayer1 = trainer == &m_Player1;
    MoveUse& moveUse = isPlayer1 ? turn[0] : turn[1];
    int& playerSwitching = isPlayer1 ? player1Switching : player2Switching;
    if (user->nextMove != nullptr){
        moveUse = MoveUse(user->nextMove, user, enemy, this);
    }
    else if (intendedMove == &MOVE_SWITCH){
        moveUse = MoveUse(&MOVE_SWITCH, user, user, this);
        playerSwitching = trainer->pickPokemon(user, enemy, this);
    }
    else{
        moveUse = MoveUse(intendedMove,user,enemy,this);
    }
}

void Battle::addMoves(const Move* move1, const Move* move2){
    isTurnOver = false;
    setMoveUse(move1, player1ActivePokemon, player2ActivePokemon, &m_Player1);
    setMoveUse(move2, player2ActivePokemon, player1ActivePokemon, &m_Player2);
    setMoveOrder();
    m_FasterPokemon = turn[0].user;
    m_SlowerPokemon = turn[1].user;
    
}



void Battle::setMoveOrder(){
    if (!compareMoves(&turn[0], &turn[1])){
        swapMoves();
    }
}

void Battle::swapMoves(){
    MoveUse temp = turn[0];
    turn[0] = turn[1];
    turn[1] = temp;
}

MoveUse* Battle::doMove(){
    MoveUse* move = &turn[moveNumber];
    if (moveNumber == 0) logMessage("---Turn " + std::to_string(turns) + "---");
    if (!move->user->isDead && (move->user == player1ActivePokemon || move->user == player2ActivePokemon)){
        move->doMove(&turn[moveNumber == 0 ? 1 : 0]);
        if (move->changeLastMoveUsed){
            move->user->lastMoveUsed = move->move;
        }
        killTheDead();
        if (move->move != &MOVE_SWITCH) raiseEvent(Event::AFTER_MOVE, EventArgs(nullptr, move));
    }
    return move;
}
Pokemon* Battle::switchPokemon(bool isPlayer1){
    int& newPokePosition = isPlayer1 ? player1Switching : player2Switching;
    Pokemon* currentPoke = isPlayer1 ? player1ActivePokemon : player2ActivePokemon;
    auto& team = isPlayer1 ? player1Team : player2Team;
    Pokemon* newPoke = &team[newPokePosition];
    assertTrue(currentPoke != newPoke, "Tried to switch in a Pokemon that is already in.");
    raiseEvent(Event::POKEMON_SWITCH, EventArgs(currentPoke, nullptr));
    setActivePokemon(isPlayer1, newPokePosition);
    raiseEvent(Event::POKEMON_ENTER, EventArgs(newPoke, nullptr));
    if (turn[1].move != &MOVE_SWITCH && turn[1].target == currentPoke) turn[1].target = newPoke;
    newPokePosition = -1;
    setPokemonHandlerOrder();
    return newPoke;
}

void Battle::switchIfNecessary(){
    if (player1Switching >= 0){
        switchPokemon(true);
    }
    if (player2Switching >= 0){
        switchPokemon(false);
    }
}

void Battle::addFieldEffect(bool side, const FieldEffect* fieldEffect){
    auto& list = side ? m_Player1FieldEffects : m_Player2FieldEffects;
    list[fieldEffect];
    fieldEffect->observer.initialize(side ? player1ActivePokemon : player2ActivePokemon, this);
}

bool Battle::sideHasFieldEffect(bool side, const FieldEffect* fieldEffect){
    auto& list = side ? m_Player1FieldEffects : m_Player2FieldEffects;
    return list.count(fieldEffect) > 0;
}

void Battle::removeFieldEffect(bool side, const FieldEffect* fieldEffect){
    auto& list = side ? m_EffectsToRemove1 : m_EffectsToRemove2;
    list.push_back(fieldEffect);
}

void Battle::removeMarkedFieldEffects(bool side){
    auto& toRemoveList = side ? m_EffectsToRemove1 : m_EffectsToRemove2;
    auto& fieldEffectList = side ? m_Player1FieldEffects : m_Player2FieldEffects;
    for (auto fieldEffect : toRemoveList){
        fieldEffectList.erase(fieldEffect);
    }
    toRemoveList.clear();
}

void Battle::raiseEvent(Event event, const EventArgs& args){
    assertTrue(event != Event::PRIORITY_END_OF_TURN, "Don't call raiseEvent with PRIORITY_END_OF_TURN.");
    bool fasterPokemonIsPlayer1 = m_FasterPokemon == player1ActivePokemon;
    Pokemon* fasterPokemon = fasterPokemonIsPlayer1 ? player1ActivePokemon : player2ActivePokemon;
    Pokemon* slowerPokemon = fasterPokemonIsPlayer1 ? player2ActivePokemon : player1ActivePokemon;
    auto& fasterPokemonFieldEffects = fasterPokemonIsPlayer1 ? m_Player1FieldEffects : m_Player2FieldEffects;
    auto& slowerPokemonFieldEffects = fasterPokemonIsPlayer1 ? m_Player2FieldEffects : m_Player1FieldEffects;

    if (weather != &WEATHER_NONE && weatherSuppressors <= 0) weather->observer.handleEvent(event, nullptr, this, args);
    if (weather != &WEATHER_NONE && event == Event::END_OF_TURN) weather->observer.handleEvent(Event::PRIORITY_END_OF_TURN, nullptr, this, args);
    killTheDead();

    m_FasterPokemon->handleEvent(event, args);
    killTheDead();
    for (auto [effect, effectState] : fasterPokemonFieldEffects){
        effect->observer.handleEvent(event, fasterPokemon, this, args);
        if (event == Event::END_OF_TURN) effect->observer.handleEvent(Event::PRIORITY_END_OF_TURN, slowerPokemon, this, args);
        killTheDead();
    }
    removeMarkedFieldEffects(fasterPokemonIsPlayer1);


    m_SlowerPokemon->handleEvent(event, args);
    killTheDead();
    for (auto [effect, effectState]: slowerPokemonFieldEffects){
        effect->observer.handleEvent(event, slowerPokemon, this, args);
        if (event == Event::END_OF_TURN) effect->observer.handleEvent(Event::PRIORITY_END_OF_TURN, slowerPokemon, this, args);
        killTheDead();
    }
    removeMarkedFieldEffects(!fasterPokemonIsPlayer1);

    switch (event)
    {
    case Event::AFTER_MOVE:
        for (int i = 0; i < 4; i++){
            if (args.moveUse->move->maxPP > 0 && args.moveUse->user->currentMoves[i] == args.moveUse->move && args.moveUse->usesPP && args.moveUse->user->currentPP[i] > 0){
                args.moveUse->user->currentPP[i] -= args.moveUse->ppUsage;
                break;
            }
        }
        break;
    case Event::END_OF_TURN:
    {
        isTurnOver = true;
        moveNumber = 0;
        turns++;

        checkIfBattleOver();

        if (!isBattleOver) {
            if (player1ActivePokemon->isDead) player1Switching = m_Player1.pickPokemon(player1ActivePokemon, player2ActivePokemon, this);
            if (player2ActivePokemon->isDead) player2Switching = m_Player2.pickPokemon(player2ActivePokemon, player1ActivePokemon, this);
        }
        break;
    }
    case Event::POKEMON_ENTER:{
        checkIfBattleOver();

        if (!isBattleOver) {
            if (player1ActivePokemon->isDead) player1Switching = m_Player1.pickPokemon(player1ActivePokemon, player2ActivePokemon, this);
            if (player2ActivePokemon->isDead) player2Switching = m_Player2.pickPokemon(player2ActivePokemon, player1ActivePokemon, this);
        }
        break;
    }

    default:
        break;
    }
}

bool Battle::trainerBlackedOut(bool player){
    auto& team = player ? player1Team : player2Team;
    for (const Pokemon& pokemon : team){
        if (!pokemon.empty && !pokemon.isDead){
            return false;
        }
    }
    return true;
}

void Battle::killPokemon(Pokemon* pokemon){
    pokemon->isDead = true;
    logPokemonFaint(pokemon->nickname + " fainted!", {.isPlayer1 = pokemon == player1ActivePokemon});
    raiseEvent(Event::POKEMON_DEATH, EventArgs(pokemon, nullptr));
}

void Battle::killTheDead(){
    if (player1ActivePokemon->shouldDie() || player2ActivePokemon->shouldDie()){
        if (player1ActivePokemon->shouldDie() && player2ActivePokemon->shouldDie()){
            killPokemon(player1ActivePokemon);
            killPokemon(player2ActivePokemon);
        }
        else{
            Pokemon* deadPoke = (player1ActivePokemon->shouldDie()) ? player1ActivePokemon : player2ActivePokemon;
            Pokemon* alivePoke = (player1ActivePokemon->shouldDie()) ? player2ActivePokemon : player1ActivePokemon;

            killPokemon(deadPoke);
            if (alivePoke->shouldDie()){
                killPokemon(alivePoke);
            }
        }
    }
}
//This function is only used for determining the order of event handlers running. It is not used for the order of moves used in the turn.
void Battle::setPokemonHandlerOrder(){
    if (player1ActivePokemon->getStat(Stat::SPEED) == player2ActivePokemon->getStat(Stat::SPEED)){
        int num = randInt(0,2);
        m_FasterPokemon = (num == 0) ? player1ActivePokemon : player2ActivePokemon;
        m_SlowerPokemon = (num == 0) ? player2ActivePokemon : player1ActivePokemon;
    }
    else{
        bool poke1IsFaster = player1ActivePokemon->getStat(Stat::SPEED) > player2ActivePokemon->getStat(Stat::SPEED);
        m_FasterPokemon = poke1IsFaster ? player1ActivePokemon : player2ActivePokemon;
        m_SlowerPokemon = poke1IsFaster ? player2ActivePokemon : player1ActivePokemon;
    }
}

void Battle::setActivePokemon(bool isPlayer1, int newPokeIndex){
    if (isPlayer1){
        Pokemon* newPokemon = &player1Team[newPokeIndex];
        player1ActivePokemon = newPokemon;
        logPokemonEnter(m_Player1.name + " sent out " + newPokemon->nickname + "!", {.isPlayer1 = isPlayer1, .newPokeIndex = newPokeIndex});
    }
    else{
        Pokemon* newPokemon = &player2Team[newPokeIndex];
        player2ActivePokemon = newPokemon;
        logPokemonEnter(m_Player2.name + " sent out " + newPokemon->nickname + "!", {.isPlayer1 = isPlayer1, .newPokeIndex = newPokeIndex});
    }
}

void Battle::checkIfBattleOver(){
    bool player1Dead = trainerBlackedOut(true);
    bool player2Dead = trainerBlackedOut(false);
    if (player1Dead || player2Dead) {
        isBattleOver = true;
        if (player1Dead && player2Dead) {
            isDraw = true;
            logMessage("It's a draw!");
        }
        else {
            winner = player1Dead ? &m_Player2 : &m_Player1;
            logMessage("The winner is " + winner->name + "!");
        }
    }
    if (turns > 200 && !isBattleOver){
        isBattleOver = true;
        isDraw = true;
        logMessage("It's a draw!");
    }
}

const char* BattleAssertionFailedException::what() const noexcept {
    return m_Message.c_str();
}

static const char* const eventTypeStrings[] = {
    "MESSAGE",
    "RANGED_ATTACK",
    "MELEE_ATTACK",
    "DAMAGE_TAKEN",
    "HEALING_RECEIVED",
    "POKEMON_ENTER",
    "POKEMON_LEAVE",
    "POKEMON_FAINT",
    "APPLY_STATUS",
    "APPLY_VOLATILE",
    "APPLY_FIELD_EFFECT",
    "REMOVE_VOLATILE",
    "REMOVE_FIELD_EFFECT",
    "WEATHER_CHANGE",
    "STAT_CHANGE",
    "DEBUG_MESSAGE"
};

template <typename T, std::size_t N>
static constexpr std::size_t arrayLength(const T (&)[N]) noexcept {
    return N;
}
static_assert(arrayLength(eventTypeStrings) == (int) LogEventType::_COUNT, "Missing event type string!");

static const char* getEventTypeString(LogEventType type){
    assert((int)type < (int)LogEventType::_COUNT);
    return eventTypeStrings[(int)type];
}

LogEventType LogEvent::type(){
    return m_type;
}
const std::string& LogEvent::message(){
    return m_message;
}
const LogEventData& LogEvent::data(){
    assert(m_type != LogEventType::MESSAGE && m_type != LogEventType::DEBUG_MESSAGE && "Tried to access log event data for message event. There isn't any. Don't do this.");
    return m_data;
}
json LogEvent::toJSON(){
    json result;
    result["message"] = m_message;
    result["type"] = getEventTypeString(m_type);
    json data;
    switch (m_type)
    {
    case LogEventType::MESSAGE:
        break;
    case LogEventType::RANGED_ATTACK:
        data["attackerIsPlayer1"] = m_data.attack.attackerIsPlayer1;
        data["damage"] = m_data.attack.damage;
        break;
    case LogEventType::MELEE_ATTACK:
        data["attackerIsPlayer1"] = m_data.attack.attackerIsPlayer1;
        data["damage"] = m_data.attack.damage;
        break;
    case LogEventType::DAMAGE_TAKEN:
        data["recipientIsPlayer1"] = m_data.damage.recipientIsPlayer1;
        data["damage"] = m_data.damage.damage;
        break;
    case LogEventType::HEALING_RECEIVED:
        data["recipientIsPlayer1"] = m_data.healing.recipientIsPlayer1;
        data["healing"] = m_data.healing.healing;
        break;
    case LogEventType::POKEMON_ENTER:
        data["isPlayer1"] = m_data.pokemonAction.isPlayer1;
        data["newPokeIndex"] = m_data.pokemonAction.newPokeIndex;
        break;
    case LogEventType::POKEMON_LEAVE:
        data["isPlayer1"] = m_data.pokemonAction.isPlayer1;
        break;
    case LogEventType::POKEMON_FAINT:
        data["isPlayer1"] = m_data.pokemonAction.isPlayer1;
        break;
    case LogEventType::APPLY_STATUS:
        data["status"] = m_data.applyStatus.status->name;
        data["appliedToPlayer1"] = m_data.applyStatus.appliedToPlayer1;
        break;
    case LogEventType::APPLY_VOLATILE:
        data["effect"] = m_data.applyVolatile.effect->name;
        data["appliedToPlayer1"] = m_data.applyVolatile.appliedToPlayer1;
        break;
    case LogEventType::REMOVE_VOLATILE:
        data["effect"] = m_data.removeVolatile.effect->name;
        data["removedFromPlayer1"] = m_data.removeVolatile.removedFromPlayer1;
        break;
    case LogEventType::REMOVE_FIELD_EFFECT:
        data["fieldEffect"] = m_data.removeFieldEffect.fieldEffect->name;
        data["removedFromPlayer1Side"] = m_data.removeFieldEffect.removedFromPlayer1Side;
        break;
    case LogEventType::APPLY_FIELD_EFFECT:
        data["fieldEffect"] = m_data.applyFieldEffect.fieldEffect->name;
        data["appliedToPlayer1Side"] = m_data.applyFieldEffect.appliedToPlayer1Side;
        break;
    case LogEventType::WEATHER_CHANGE:
        data["weather"] = m_data.weatherChange.weather->name;
        break;
    case LogEventType::STAT_CHANGE:
        data["stat"] = (int) m_data.statChange.stat;
        data["change"] = (int) m_data.statChange.change;
        break;
    case LogEventType::DEBUG_MESSAGE:
        break;
    default:
        break;
    }
    result["data"] = data;
    return result;
}

LogEvent::LogEvent(LogEventType type, const std::string& message) :                                     m_type{type}, m_message{message}{}
LogEvent::LogEvent(LogEventType type, const std::string& message, const AttackData& data) :             m_type{type}, m_message{message}{ this->m_data.attack = data; }
LogEvent::LogEvent(LogEventType type, const std::string& message, const DamageData& data) :             m_type{type}, m_message{message}{ this->m_data.damage = data; }
LogEvent::LogEvent(LogEventType type, const std::string& message, const HealingData& data) :            m_type{type}, m_message{message}{ this->m_data.healing = data; }
LogEvent::LogEvent(LogEventType type, const std::string& message, const PokemonActionData& data) :      m_type{type}, m_message{message}{ this->m_data.pokemonAction = data; }
LogEvent::LogEvent(LogEventType type, const std::string& message, const ApplyStatusData& data) :        m_type{type}, m_message{message}{ this->m_data.applyStatus = data; }
LogEvent::LogEvent(LogEventType type, const std::string& message, const ApplyVolatileData& data) :      m_type{type}, m_message{message}{ this->m_data.applyVolatile = data; }
LogEvent::LogEvent(LogEventType type, const std::string& message, const StatChangeData& data) :         m_type{type}, m_message{message}{ this->m_data.statChange = data; }
LogEvent::LogEvent(LogEventType type, const std::string& message, const ApplyFieldEffectData& data) :   m_type{type}, m_message{message}{ this->m_data.applyFieldEffect = data; }
LogEvent::LogEvent(LogEventType type, const std::string& message, const WeatherChangeData& data) :      m_type{type}, m_message{message}{ this->m_data.weatherChange = data; }
LogEvent::LogEvent(LogEventType type, const std::string& message, const RemoveVolatileData& data) :     m_type{type}, m_message{message}{ this->m_data.removeVolatile = data; }
LogEvent::LogEvent(LogEventType type, const std::string& message, const RemoveFieldEffectData& data) :  m_type{type}, m_message{message}{ this->m_data.removeFieldEffect = data; }

void Battle::logMessage(std::string_view message){
    if (!doLogging) return;
    m_EventLog.emplace_back(LogEventType::MESSAGE, std::string(message));
}
void Battle::logRangedAttack(std::string_view message, const AttackData& data){
    if (!doLogging) return;
    m_EventLog.emplace_back(LogEventType::RANGED_ATTACK, std::string(message), data);
}
void Battle::logMeleeAttack(std::string_view message, const AttackData& data){
    if (!doLogging) return;
    m_EventLog.emplace_back(LogEventType::MELEE_ATTACK, std::string(message), data);
}
void Battle::logDamageTaken(std::string_view message, const DamageData& data){
    if (!doLogging) return;
    m_EventLog.emplace_back(LogEventType::DAMAGE_TAKEN, std::string(message), data);
}
void Battle::logHealing(std::string_view message, const HealingData& data){
    if (!doLogging) return;
    m_EventLog.emplace_back(LogEventType::HEALING_RECEIVED, std::string(message), data);
}
void Battle::logPokemonEnter(std::string_view message, const PokemonActionData& data){
    if (!doLogging) return;
    m_EventLog.emplace_back(LogEventType::POKEMON_ENTER, std::string(message), data);
}
void Battle::logPokemonLeave(std::string_view message, const PokemonActionData& data){
    if (!doLogging) return;
    m_EventLog.emplace_back(LogEventType::POKEMON_LEAVE, std::string(message), data);
}
void Battle::logPokemonFaint(std::string_view message, const PokemonActionData& data){
    if (!doLogging) return;
    m_EventLog.emplace_back(LogEventType::POKEMON_FAINT, std::string(message), data);
}
void Battle::logApplyStatus(std::string_view message, const ApplyStatusData& data){
    if (!doLogging) return;
    m_EventLog.emplace_back(LogEventType::APPLY_STATUS, std::string(message), data);
}
void Battle::logApplyVolatile(std::string_view message, const ApplyVolatileData& data){
    if (!doLogging) return;
    m_EventLog.emplace_back(LogEventType::APPLY_VOLATILE, std::string(message), data);
}
void Battle::logApplyFieldEffect(std::string_view message, const ApplyFieldEffectData& data){
    if (!doLogging) return;
    m_EventLog.emplace_back(LogEventType::APPLY_FIELD_EFFECT, std::string(message), data);
}
void Battle::logRemoveVolatile(std::string_view message, const RemoveVolatileData& data){
    if (!doLogging) return;
    m_EventLog.emplace_back(LogEventType::REMOVE_VOLATILE, std::string(message), data);
}
void Battle::logRemoveFieldEffect(std::string_view message, const RemoveFieldEffectData& data){
    if (!doLogging) return;
    m_EventLog.emplace_back(LogEventType::REMOVE_FIELD_EFFECT, std::string(message), data);
}
void Battle::logChangeWeather(std::string_view message, const WeatherChangeData& data){
    if (!doLogging) return;
    m_EventLog.emplace_back(LogEventType::WEATHER_CHANGE, std::string(message), data);
}
void Battle::logStatChange(std::string_view message, const StatChangeData& data){
    if (!doLogging) return;
    m_EventLog.emplace_back(LogEventType::STAT_CHANGE, std::string(message), data);
}

const std::vector<LogEvent>& Battle::eventLog(){
    return m_EventLog;
}

static json trainerEventJson(const TrainerInfo& trainer, const std::array<Pokemon, 6>& team){
    json result;
    result["name"] = trainer.name;
    std::vector<json> teamJSON;
    for (auto& poke : team){
        if (poke.empty) continue;
        json data;
        data["species"] = poke.species->name;
        data["name"] = poke.nickname;
        data["level"] = poke.level;
        data["gender"] = genderToString.at(poke.getGender());
        data["maxHealth"] = poke.getStatRaw(Stat::HP);
        teamJSON.push_back(data);
    }
    result["team"] = teamJSON;
    return result;
}

json Battle::eventsJson(){
    json result;
    std::vector<json> events;
    for (auto& event : m_EventLog){
        events.push_back(event.toJSON());
    }
    result["events"] = events;
    result["trainer1"] = trainerEventJson(m_Player1, player1Team);
    result["trainer2"] = trainerEventJson(m_Player2, player2Team);
    return result;
}

std::string Battle::textLog(){
    std::string result = "";
    for (auto& event : m_EventLog){
        result += event.message() + "\n";
    }
    return result;
}