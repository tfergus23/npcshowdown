#include "battle/Battle.hpp"
#include <random>
#include "data/Moves.hpp"
#include "assert.h"
#define DEBUG_LOG 1

Battle::Battle(const Trainer& trainer1, const Trainer& trainer2, int seed) :
m_Player1{trainer1},
m_Player2{trainer2},
m_Seed{seed},
player1Team{{Pokemon(&(trainer1.teamBlueprint[0]), this),Pokemon(&(trainer1.teamBlueprint[1]), this),Pokemon(&(trainer1.teamBlueprint[2]), this),Pokemon(&(trainer1.teamBlueprint[3]), this),Pokemon(&(trainer1.teamBlueprint[4]), this),Pokemon(&(trainer1.teamBlueprint[5]), this)}},
player2Team{{Pokemon(&(trainer2.teamBlueprint[0]), this),Pokemon(&(trainer2.teamBlueprint[1]), this),Pokemon(&(trainer2.teamBlueprint[2]), this),Pokemon(&(trainer2.teamBlueprint[3]), this),Pokemon(&(trainer2.teamBlueprint[4]), this),Pokemon(&(trainer2.teamBlueprint[5]), this)}}
{
    log("Seed: " + std::to_string(seed));
    setActivePokemon(true, &(player1Team[0]));
    setActivePokemon(false, &(player2Team[0]));
    setPokemonSpeedOrder();
    raisePokemonEnter(m_FasterPokemon);
    raisePokemonEnter(m_SlowerPokemon);
}

void Battle::log(const std::string& message){
    battleLog += message + "\n";
}

void Battle::debug(const std::string& message){
#if DEBUG_LOG
    battleLog += message + "\n";
#endif
}



int Battle::randInt(int min, int max){
    int rand = m_Distribution(m_Generator);
    int diff = max - min;
    int offset = rand % diff;
    return min + offset;
}

int Battle::getSeed(){
    return m_Seed;
}

void Battle::addMoves(std::vector<MoveUse>& actions){
    isTurnOver = false;
    m_Turn = actions;
    std::sort(m_Turn.begin(), m_Turn.end(), compareMoves);
    m_FasterPokemon = m_Turn[0].user;
    m_SlowerPokemon = m_Turn[1].user;
    
}

MoveUse* Battle::doMove(){
    MoveUse* move = &m_Turn[moveNumber];
    if (moveNumber == 0) log("----------------------Turn " + std::to_string(turns) + "----------------------");
    if (!move->user->isDead && (move->user == player1ActivePokemon || move->user == player2ActivePokemon)){
        move->doMove(&m_Turn[moveNumber == 0 ? 1 : 0]);
        if (move->changeLastMoveUsed){
            move->user->lastMoveUsed = move->move;
        }
        if (move->move != &MOVE_SWITCH) raiseAfterMove(move);
    }
    return move;
}
Pokemon* Battle::switchPokemon(bool isPlayer1, int newPokePosition){
    Pokemon* currentPoke = isPlayer1 ? player1ActivePokemon : player2ActivePokemon;
    auto& team = isPlayer1 ? player1Team : player2Team;
    Pokemon* newPoke = &team[newPokePosition];
    assert(currentPoke != newPoke);
    raisePokemonSwitch(currentPoke);
    setActivePokemon(isPlayer1, newPoke);
    raisePokemonEnter(newPoke);
    if (m_Turn[1].move != &MOVE_SWITCH && m_Turn[1].target == currentPoke) m_Turn[1].target = newPoke;
    if (isPlayer1) player1Switching = false;
    else player2Switching = false;
    return newPoke;
}

void raiseBeforeMove(MoveUse* moveUse);
void raiseAfterMove(MoveUse* moveUse);
void raiseEndOfTurn();
void raisePokemonEnter(Pokemon* enteringPokemon);
void raisePokemonSwitch(Pokemon* switchingPokemon);
void raisePokemonDeath(Pokemon* dyingPokemon);
void killTheDead();
void setPokemonSpeedOrder();
void setActivePokemon(bool isPlayer1, Pokemon* newPokemon);
bool checkForOver();
std::string currentStatus();