#pragma once
#include "battle/Trainer.hpp"
#include "battle/Pokemon.hpp"
#include "battle/Weather.hpp"
#include "battle/FieldEffect.hpp"
#include <random>

class Battle{
public:
    Trainer* getPlayer1();
    Trainer* getPlayer2();
    std::array<Pokemon,6> player1Team;
    std::array<Pokemon,6> player2Team;
    Pokemon* player1ActivePokemon;
    Pokemon* player2ActivePokemon;
    bool player1Switching = false;
    bool player2Switching = false;
    bool isTurnOver = false;
    const Weather* weather;
    int turns = 1;
    bool isBattleOver = false;
    Trainer* winner;
    std::string battleLog = "";
    int weatherSuppressors = 0;
    int moveNumber = 0;

    //Used by the Use2MovesThenSwitch AI to count how many moves it used so far
    int player1SwitchCounter = 0;
    int player2SwitchCounter = 0;

    int randInt(int min, int max);
    int getSeed();

    Battle(const Trainer& trainer1, const Trainer& trainer2, int seed);
    void addMoves(std::vector<MoveUse>& actions);
    MoveUse* doMove();
    Pokemon* switchPokemon(bool isPlayer1, int newPokePosition);
    void addFieldEffect(bool side, const FieldEffect* fieldEffect);
    bool sideHasFieldEffect(bool side, const FieldEffect* fieldEffect);
    EffectState* getFieldEffectState(bool side, const FieldEffect* fieldEffect);
    void removeFieldEffect(bool side, const FieldEffect* fieldEffect);
    void log(const std::string& str);
    void debug(const std::string& str);

    void raiseBeforeMove(MoveUse* moveUse);
    void killTheDead();
    void setActivePokemon(bool isPlayer1, Pokemon* newPokemon);
    bool checkForOver();
    std::string currentStatus();
private:
    Trainer m_Player1;
    Trainer m_Player2;
    std::default_random_engine m_Generator;
    std::uniform_int_distribution<int> m_Distribution;
    int m_Seed;
    Pokemon* m_FasterPokemon;
    Pokemon* m_SlowerPokemon;
    std::vector<MoveUse> m_Turn;
    std::unordered_map<const FieldEffect*, EffectState> m_Player1FieldEffects;
    std::unordered_map<const FieldEffect*, EffectState> m_Player2FieldEffects;
    std::vector<const FieldEffect*> m_EffectsToRemove1;
    std::vector<const FieldEffect*> m_EffectsToRemove2;
    void setPokemonSpeedOrder();

    void raiseAfterMove(MoveUse* moveUse);
    void raiseEndOfTurn();
    void raisePokemonEnter(Pokemon* enteringPokemon);
    void raisePokemonSwitch(Pokemon* switchingPokemon);
    void raisePokemonDeath(Pokemon* dyingPokemon);
};

void simulateBattle(Battle* battle);