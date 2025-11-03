#pragma once
#include "sim/battle/Observer.hpp"
#include "sim/battle/TrainerInfo.hpp"
#include "sim/battle/Trainer.hpp"
#include "sim/battle/Pokemon.hpp"
#include "sim/battle/Weather.hpp"
#include "sim/data/Weathers.hpp"
#include "sim/battle/FieldEffect.hpp"
#include "sim/battle/MoveUse.hpp"
#include "sim/battle/ObserverState.hpp"
#include "sim/battle/Event.hpp"
#include "sim/battle/EventArgs.hpp"
#include <random>
#include <unordered_map>
#include "sim/data/Volatiles.hpp"
#include "sim/data/FieldEffects.hpp"

class Trainer;

struct DebugOptions{
    bool debugLogging = false;
    bool averageDamage = false;
};

enum class LogEventType {
    MESSAGE,
    RANGED_ATTACK,
    MELEE_ATTACK,
    DAMAGE_TAKEN,
    HEALING_RECEIVED,
    POKEMON_ENTER,
    POKEMON_LEAVE,
    POKEMON_FAINT,
    APPLY_STATUS,
    APPLY_VOLATILE,
    APPLY_FIELD_EFFECT,
    REMOVE_VOLATILE,
    REMOVE_FIELD_EFFECT,
    WEATHER_CHANGE,
    STAT_CHANGE,
    DEBUG_MESSAGE,
    _COUNT
};

struct AttackData{
    bool attackerIsPlayer1 = true;
    int damage = 0;
};

struct DamageData{
    bool recipientIsPlayer1 = true;
    int damage = 0;
};

struct HealingData {
    bool recipientIsPlayer1 = true;
    int healing = 0;
};

struct PokemonActionData{
    bool isPlayer1 = true;
    int newPokeIndex = -1;
};

struct ApplyStatusData {
    bool appliedToPlayer1 = true;
    const Status* status = &STATUS_NONE;
};

struct ApplyVolatileData {
    bool appliedToPlayer1 = true;
    const Volatile* effect = &VOLATILE_NONE;
};

struct ApplyFieldEffectData {
    bool appliedToPlayer1Side = true;
    const FieldEffect* fieldEffect = &FIELD_EFFECT_NONE;
};

struct RemoveVolatileData {
    bool removedFromPlayer1 = true;
    const Volatile* effect = &VOLATILE_NONE;
};

struct RemoveFieldEffectData {
    bool removedFromPlayer1Side = true;
    const FieldEffect* fieldEffect = &FIELD_EFFECT_NONE;
};

struct WeatherChangeData {
    const Weather* weather = &WEATHER_NONE;
};

struct StatChangeData {
    Stat stat = Stat::ATTACK;
    int change = 0;
};


union LogEventData {
    AttackData attack;
    DamageData damage;
    HealingData healing;
    PokemonActionData pokemonAction;
    ApplyStatusData applyStatus;
    ApplyVolatileData applyVolatile;
    StatChangeData statChange;
    ApplyFieldEffectData applyFieldEffect;
    WeatherChangeData weatherChange;
    RemoveVolatileData removeVolatile;
    RemoveFieldEffectData removeFieldEffect;

    LogEventData(){}
};

struct LogEvent {
public:
    LogEventType type();
    const std::string& message();
    const LogEventData& data();
    json toJSON();
    LogEvent(LogEventType type, const std::string& message);
    LogEvent(LogEventType type, const std::string& message, const AttackData& data);
    LogEvent(LogEventType type, const std::string& message, const DamageData& data);
    LogEvent(LogEventType type, const std::string& message, const HealingData& data);
    LogEvent(LogEventType type, const std::string& message, const PokemonActionData& data);
    LogEvent(LogEventType type, const std::string& message, const ApplyStatusData& data);
    LogEvent(LogEventType type, const std::string& message, const ApplyVolatileData& data);
    LogEvent(LogEventType type, const std::string& message, const ApplyFieldEffectData& data);
    LogEvent(LogEventType type, const std::string& message, const StatChangeData& data);
    LogEvent(LogEventType type, const std::string& message, const WeatherChangeData& data);
    LogEvent(LogEventType type, const std::string& message, const RemoveVolatileData& data);
    LogEvent(LogEventType type, const std::string& message, const RemoveFieldEffectData& data);
private:
    LogEventType m_type;
    std::string m_message;
    LogEventData m_data;
};

class Battle{
public:
    DebugOptions debugOptions;
    std::array<Pokemon,6> player1Team;
    std::array<Pokemon,6> player2Team;
    Pokemon* player1ActivePokemon;
    Pokemon* player2ActivePokemon;
    int player1Switching = -1; //-1 indicates no switch. >= 0 indicates position of pokemon
    int player2Switching = -1;
    MoveUse turn[2];
    bool isTurnOver = false;
    const Weather* weather = &WEATHER_NONE;
    int turns = 1;
    bool isBattleOver = false;
    bool isDraw = false;
    const TrainerInfo* winner;
    int weatherSuppressors = 0;
    int moveNumber = 0;
    bool doLogging = true;
    bool invalid = false;
    std::string errorMessage = "";
    //Used by the Use2MovesThenSwitch AI to count how many moves it used so far
    int player1SwitchCounter = 0;
    int player2SwitchCounter = 0;

    Battle(const Trainer& trainer1, const Trainer& trainer2, size_t seed, const DebugOptions&& debugOptions = DebugOptions());
    const TrainerInfo* getPlayer1() const;
    const TrainerInfo* getPlayer2() const;
    int randInt(int min, int max);
    size_t getSeed();
    void addMoves(const Move* move1, const Move* move2);
    MoveUse* doMove();
    Pokemon* switchPokemon(bool isPlayer1);
    void switchIfNecessary();
    void addFieldEffect(bool side, const FieldEffect* fieldEffect);
    bool sideHasFieldEffect(bool side, const FieldEffect* fieldEffect);
    ObserverState* getFieldEffectState(bool side, const FieldEffect* fieldEffect);
    void removeFieldEffect(bool side, const FieldEffect* fieldEffect);
    void logMessage(std::string_view message);
    void logRangedAttack(std::string_view message, const AttackData& data);
    void logMeleeAttack(std::string_view message, const AttackData& data);
    void logDamageTaken(std::string_view message, const DamageData& data);
    void logHealing(std::string_view message, const HealingData& data);
    void logPokemonEnter(std::string_view message, const PokemonActionData& data);
    void logPokemonLeave(std::string_view message, const PokemonActionData& data);
    void logPokemonFaint(std::string_view message, const PokemonActionData& data);
    void logApplyStatus(std::string_view message, const ApplyStatusData& data);
    void logApplyVolatile(std::string_view message, const ApplyVolatileData& data);
    void logApplyFieldEffect(std::string_view message, const ApplyFieldEffectData& data);
    void logRemoveVolatile(std::string_view message, const RemoveVolatileData& data);
    void logRemoveFieldEffect(std::string_view message, const RemoveFieldEffectData& data);
    void logChangeWeather(std::string_view message, const WeatherChangeData& data);
    void logStatChange(std::string_view message, const StatChangeData& data);
    const std::vector<LogEvent>& eventLog();
    json eventsJson();
    std::string textLog();
    void debug(std::string_view str);
    void assertTrue(bool condition, std::string_view message = "");
    void raiseEvent(Event event, const EventArgs& args);
    void killTheDead();
    void setActivePokemon(bool isPlayer1, int newPokeIndex);
    void simulate();
private:
    TrainerInfo m_Player1;
    TrainerInfo m_Player2;
    bool trainerBlackedOut(bool player);
    void setMoveUse(const Move* intendedMove, Pokemon* user, Pokemon* enemy, const TrainerInfo* trainer);
    size_t m_Seed;
    std::default_random_engine m_Generator;
    Pokemon* m_FasterPokemon;
    Pokemon* m_SlowerPokemon;
    std::unordered_map<const FieldEffect*, ObserverState> m_Player1FieldEffects;
    std::unordered_map<const FieldEffect*, ObserverState> m_Player2FieldEffects;
    std::vector<const FieldEffect*> m_EffectsToRemove1;
    std::vector<const FieldEffect*> m_EffectsToRemove2;
    std::vector<LogEvent> m_EventLog;

    void removeMarkedFieldEffects(bool side);
    void setPokemonHandlerOrder();
    void setMoveOrder();
    void swapMoves();
    void killPokemon(Pokemon* pokemon);
};

class BattleAssertionFailedException : public std::exception {
public:
	BattleAssertionFailedException(std::string_view message) : m_Message{message} {}

	const char* what() const noexcept override;
private:
	const std::string m_Message;
};