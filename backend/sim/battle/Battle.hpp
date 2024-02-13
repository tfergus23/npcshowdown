#pragma once
#include "sim/battle/Observer.hpp"
#include "sim/battle/TrainerInfo.hpp"
#include "sim/battle/Trainer.hpp"
#include "sim/battle/Pokemon.hpp"
#include "sim/battle/Weather.hpp"
#include "sim/data/Weathers.hpp"
#include "sim/battle/FieldEffect.hpp"
#include "sim/battle/MoveUse.hpp"
#include "sim/battle/EffectState.hpp"
#include "sim/battle/Event.hpp"
#include "sim/battle/EventArgs.hpp"
#include <random>
#include <unordered_map>

class Trainer;

class Battle{
public:
    const TrainerInfo* getPlayer1();
    const TrainerInfo* getPlayer2();
    std::array<Pokemon,6> player1Team;
    std::array<Pokemon,6> player2Team;
    Pokemon* player1ActivePokemon;
    Pokemon* player2ActivePokemon;
    int player1Switching = -1; //-1 indicates no switch. >= 0 indicates position of pokemon
    int player2Switching = -1;
    bool isTurnOver = false;
    const Weather* weather = WEATHER_NONE;
    int turns = 1;
    bool isBattleOver = false;
    bool isDraw = false;
    const TrainerInfo* winner;
    std::string battleLog = "";
    int weatherSuppressors = 0;
    int moveNumber = 0;
    bool doLogging = true;
    bool invalid = false;

    //Used by the Use2MovesThenSwitch AI to count how many moves it used so far
    int player1SwitchCounter = 0;
    int player2SwitchCounter = 0;

    int randInt(int min, int max);
    int getSeed();

    Battle(const Trainer& trainer1, const Trainer& trainer2, size_t seed);
    void addMoves(const Move* move1, const Move* move2);
    MoveUse* doMove();
    Pokemon* switchPokemon(bool isPlayer1);
    void switchIfNecessary();
    void addFieldEffect(bool side, const FieldEffect* fieldEffect);
    bool sideHasFieldEffect(bool side, const FieldEffect* fieldEffect);
    EffectState* getFieldEffectState(bool side, const FieldEffect* fieldEffect);
    void removeFieldEffect(bool side, const FieldEffect* fieldEffect);
    void log(std::string_view str);
    void debug(std::string_view str);
    void assertTrue(bool condition, std::string_view message = "");

    //void raiseBeforeMove(MoveUse* moveUse);
    //void raiseEndOfTurn();
    void raiseEvent(Event event, const EventArgs& args);
    void killTheDead();
    void setActivePokemon(bool isPlayer1, Pokemon* newPokemon);
    void logCurrentStatus();
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
    MoveUse m_Turn[2];
    std::unordered_map<const FieldEffect*, EffectState> m_Player1FieldEffects;
    std::unordered_map<const FieldEffect*, EffectState> m_Player2FieldEffects;
    std::vector<const FieldEffect*> m_EffectsToRemove1;
    std::vector<const FieldEffect*> m_EffectsToRemove2;
    void removeMarkedFieldEffects(bool side);
    void setPokemonHandlerOrder();
    void setMoveOrder();
    void swapMoves();
    void killPokemon(Pokemon* pokemon);
};

//void simulateBattle(Battle* battle);

const bool IS_PLAYER_ONE = true;
const bool IS_PLAYER_TWO = false;


class BattleAssertionFailedException : public std::exception {
public:
	BattleAssertionFailedException(std::string_view message) : m_Message{message} {}

	const char* what() const noexcept override;
private:
	const std::string m_Message;
};