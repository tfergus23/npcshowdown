#include "sim/battle/Battle.hpp"
#include <random>
#include "sim/data/Moves.hpp"
#include "sim/data/Weathers.hpp"
#include "sim/data/Items.hpp"
#define DEBUG_LOG 1

Battle::Battle(Trainer& trainer1, Trainer& trainer2, int seed) :
m_Player1{&trainer1},
m_Player2{&trainer2},
m_Seed{seed},
player1Team{{Pokemon(&(trainer1.teamBlueprint[0]), this),Pokemon(&(trainer1.teamBlueprint[1]), this),Pokemon(&(trainer1.teamBlueprint[2]), this),Pokemon(&(trainer1.teamBlueprint[3]), this),Pokemon(&(trainer1.teamBlueprint[4]), this),Pokemon(&(trainer1.teamBlueprint[5]), this)}},
player2Team{{Pokemon(&(trainer2.teamBlueprint[0]), this),Pokemon(&(trainer2.teamBlueprint[1]), this),Pokemon(&(trainer2.teamBlueprint[2]), this),Pokemon(&(trainer2.teamBlueprint[3]), this),Pokemon(&(trainer2.teamBlueprint[4]), this),Pokemon(&(trainer2.teamBlueprint[5]), this)}}
{
    log("Seed: " + std::to_string(seed));
    setActivePokemon(IS_PLAYER_ONE, &(player1Team[0]));
    setActivePokemon(IS_PLAYER_TWO, &(player2Team[0]));
    setPokemonSpeedOrder();
    raisePokemonEnter(m_FasterPokemon);
    raisePokemonEnter(m_SlowerPokemon);
}

void Battle::log(const std::string& message){
    if (doLogging)
    battleLog += message + "\n";
}

void Battle::debug(const std::string& message){
#if DEBUG_LOG
    if (doLogging)
    battleLog += message + "\n";
#endif
}

void Battle::assert(bool condition, const std::string& message){
    if (!condition){
        log("Assertion failed! Stopping battle.");
        throw BattleAssertionFailedException(message);
    }
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

void Battle::setMoveUse(const Move* intendedMove, Pokemon* user, Pokemon* enemy, Trainer* trainer){
    bool isPlayer1 = trainer == m_Player1;
    MoveUse& moveUse = isPlayer1 ? m_Turn[0] : m_Turn[1];
    int& playerSwitching = isPlayer1 ? player1Switching : player2Switching;
    if (user->nextMove != nullptr){
        moveUse = MoveUse(user->nextMove, user, enemy, this);
    }
    else if (intendedMove == &MOVE_SWITCH){
        moveUse = MoveUse(&MOVE_SWITCH, user, user, this);
        playerSwitching = trainer->pickPokemon(user, enemy, this);
    }
    else if (user->outOfPP()){
        moveUse = MoveUse(&MOVE_STRUGGLE, user, enemy, this);
    }
    else{
        moveUse = MoveUse(intendedMove,user,enemy,this);
    }
}

void Battle::addMoves(const Move* move1, const Move* move2){
    isTurnOver = false;
    setMoveUse(move1, player1ActivePokemon, player2ActivePokemon, m_Player1);
    setMoveUse(move2, player2ActivePokemon, player1ActivePokemon, m_Player2);
    setMoveOrder();
    m_FasterPokemon = m_Turn[0].user;
    m_SlowerPokemon = m_Turn[1].user;
    
}



void Battle::setMoveOrder(){
    if (!compareMoves(&m_Turn[0], &m_Turn[1])){
        swapMoves();
    }
}

void Battle::swapMoves(){
    MoveUse temp = m_Turn[0];
    m_Turn[0] = m_Turn[1];
    m_Turn[1] = temp;
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
Pokemon* Battle::switchPokemon(bool isPlayer1){
    int& newPokePosition = isPlayer1 ? player1Switching : player2Switching;
    Pokemon* currentPoke = isPlayer1 ? player1ActivePokemon : player2ActivePokemon;
    auto& team = isPlayer1 ? player1Team : player2Team;
    Pokemon* newPoke = &team[newPokePosition];
    assert(currentPoke != newPoke, "Tried to switch in a Pokemon that is already in.");
    raisePokemonSwitch(currentPoke);
    setActivePokemon(isPlayer1, newPoke);
    raisePokemonEnter(newPoke);
    if (m_Turn[1].move != &MOVE_SWITCH && m_Turn[1].target == currentPoke) m_Turn[1].target = newPoke;
    if (isPlayer1) player1Switching = false;
    else player2Switching = false;
    newPokePosition = -1;
    return newPoke;
}

void Battle::switchIfNecessary(){
    if (player1Switching >= 0){
        switchPokemon(IS_PLAYER_ONE);
    }
    if (player2Switching >= 0){
        switchPokemon(IS_PLAYER_TWO);
    }
}

void Battle::addFieldEffect(bool side, const FieldEffect* fieldEffect){
    auto& list = side ? m_Player1FieldEffects : m_Player2FieldEffects;
    list[fieldEffect];
}
bool Battle::sideHasFieldEffect(bool side, const FieldEffect* fieldEffect){
    auto& list = side ? m_Player1FieldEffects : m_Player2FieldEffects;
    return list.count(fieldEffect) > 0;
}
EffectState* Battle::getFieldEffectState(bool side, const FieldEffect* fieldEffect){
    auto& list = side ? m_Player1FieldEffects : m_Player2FieldEffects;
    return &list[fieldEffect];
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

void Battle::raiseBeforeMove(MoveUse* moveUse){
    bool fasterPokemonIsPlayer1 = m_FasterPokemon == player1ActivePokemon;
    auto& fasterPokemonFieldEffects = fasterPokemonIsPlayer1 ? m_Player1FieldEffects : m_Player2FieldEffects;
    auto& slowerPokemonFieldEffects = fasterPokemonIsPlayer1 ? m_Player2FieldEffects : m_Player1FieldEffects;

    if (weather != WEATHER_NONE && weatherSuppressors <= 0) weather->beforeMove(moveUse);

    m_FasterPokemon->beforeMove(moveUse);
    for (auto effect : fasterPokemonFieldEffects){
        effect.first->beforeMove(moveUse);
    }
    removeMarkedFieldEffects(fasterPokemonIsPlayer1);


    m_SlowerPokemon->beforeMove(moveUse);
    for (auto effect: slowerPokemonFieldEffects){
        effect.first->beforeMove(moveUse);
    }
    removeMarkedFieldEffects(!fasterPokemonIsPlayer1);

}
void Battle::raiseAfterMove(MoveUse* moveUse){
    bool fasterPokemonIsPlayer1 = m_FasterPokemon == player1ActivePokemon;
    auto& fasterPokemonFieldEffects = fasterPokemonIsPlayer1 ? m_Player1FieldEffects : m_Player2FieldEffects;
    auto& slowerPokemonFieldEffects = fasterPokemonIsPlayer1 ? m_Player2FieldEffects : m_Player1FieldEffects;

    if (weather != WEATHER_NONE && weatherSuppressors <= 0) weather->afterMove(moveUse);
    killTheDead();

    m_FasterPokemon->afterMove(moveUse);
    for (auto effect : fasterPokemonFieldEffects){
        effect.first->afterMove(moveUse);
        killTheDead();
    }
    removeMarkedFieldEffects(fasterPokemonIsPlayer1);


    m_SlowerPokemon->afterMove(moveUse);
    for (auto effect: slowerPokemonFieldEffects){
        effect.first->afterMove(moveUse);
        killTheDead();
    }
    removeMarkedFieldEffects(!fasterPokemonIsPlayer1);

    for (int i = 0; i < 4; i++){
        if (moveUse->move->maxPP > 0 && moveUse->user->currentMoves[i] == moveUse->move && moveUse->usesPP){
            moveUse->user->currentPP[i] -= moveUse->ppUsage;
        }
    }
    /*
    if (moveUse->target != moveUse->user){
        moveUse->target->lastMoveUsedAgainstMe = moveUse; //TODO this will probably need to change
    }
    else{
        Pokemon* opponent = moveUse->user == player1ActivePokemon ? player2ActivePokemon : player1ActivePokemon;
        opponent->lastMoveUsedAgainstMe = nullptr;
    }
    */
}
void raiseEndOfTurn();
void raisePokemonEnter(Pokemon* enteringPokemon);
void raisePokemonSwitch(Pokemon* switchingPokemon);
void raisePokemonDeath(Pokemon* dyingPokemon);
void killTheDead();
void setPokemonSpeedOrder();
void setActivePokemon(bool isPlayer1, Pokemon* newPokemon);
bool checkForOver();
std::string currentStatus();

void simulateBattle(Battle* battle){
    try{
        while (!battle->isBattleOver){
            const Move* player1Move = battle->getPlayer1()->pickMove(battle->player1ActivePokemon, battle->player2ActivePokemon, battle);
            const Move* player2Move = battle->getPlayer2()->pickMove(battle->player2ActivePokemon, battle->player1ActivePokemon, battle);
            battle->addMoves(player1Move, player2Move);
            while (!battle->isTurnOver){
                battle->doMove();
                battle->switchIfNecessary();
                if (battle->moveNumber == 1){
                    battle->raiseEndOfTurn();
                    battle->switchIfNecessary();
                    battle->log(battle->currentStatus());
                }
                else{
                    battle->moveNumber++;
                }
            }
        }
    }
    catch (const std::exception& e){
        battle->invalid = true;
        battle->isTurnOver = true;
        battle->isBattleOver = true;
        battle->winner = battle->getPlayer1(); //This could probably remain nullptr, but I'll keep it this for now
        battle->log(e.what());
    }
}

const char* BattleAssertionFailedException::what() const noexcept {
    return m_Message.c_str();
}