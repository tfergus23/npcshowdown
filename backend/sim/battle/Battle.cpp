#include "sim/battle/Battle.hpp"
#include <random>
#include "sim/data/Moves.hpp"
#include "sim/data/Weathers.hpp"
#include "sim/data/Items.hpp"
#include <iostream>
#define DEBUG_LOG 0

Battle::Battle(Trainer& trainer1, Trainer& trainer2, int seed) :
m_Player1{&trainer1},
m_Player2{&trainer2},
m_Seed{seed},
player1Team{{Pokemon(&(trainer1.teamBlueprint[0]), this),Pokemon(&(trainer1.teamBlueprint[1]), this),Pokemon(&(trainer1.teamBlueprint[2]), this),Pokemon(&(trainer1.teamBlueprint[3]), this),Pokemon(&(trainer1.teamBlueprint[4]), this),Pokemon(&(trainer1.teamBlueprint[5]), this)}},
player2Team{{Pokemon(&(trainer2.teamBlueprint[0]), this),Pokemon(&(trainer2.teamBlueprint[1]), this),Pokemon(&(trainer2.teamBlueprint[2]), this),Pokemon(&(trainer2.teamBlueprint[3]), this),Pokemon(&(trainer2.teamBlueprint[4]), this),Pokemon(&(trainer2.teamBlueprint[5]), this)}},
m_Generator{std::default_random_engine(m_Seed)}
{
    log("Seed: " + std::to_string(seed));
    setActivePokemon(IS_PLAYER_ONE, &(player1Team[0]));
    setActivePokemon(IS_PLAYER_TWO, &(player2Team[0]));
    setPokemonHandlerOrder();
    raiseEvent(POKEMON_ENTER, EventArgs(m_FasterPokemon, nullptr));
    raiseEvent(POKEMON_ENTER, EventArgs(m_SlowerPokemon, nullptr));
}

Trainer* Battle::getPlayer1() {
    return m_Player1;
}
Trainer* Battle::getPlayer2() {
    return m_Player2;
}

void Battle::log(const std::string& message){
    if (doLogging)
    battleLog += message + "\n";
    //std::cout << message << '\n';
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
    std::uniform_int_distribution<int> distribution(min, max-1);
    int dice_roll = distribution(m_Generator);
    return dice_roll;
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
        if (move->move != &MOVE_SWITCH) raiseEvent(AFTER_MOVE, EventArgs(nullptr, move));
    }
    return move;
}
Pokemon* Battle::switchPokemon(bool isPlayer1){
    int& newPokePosition = isPlayer1 ? player1Switching : player2Switching;
    Pokemon* currentPoke = isPlayer1 ? player1ActivePokemon : player2ActivePokemon;
    auto& team = isPlayer1 ? player1Team : player2Team;
    Pokemon* newPoke = &team[newPokePosition];
    assert(currentPoke != newPoke, "Tried to switch in a Pokemon that is already in.");
    raiseEvent(POKEMON_SWITCH, EventArgs(currentPoke, nullptr));
    setActivePokemon(isPlayer1, newPoke);
    raiseEvent(POKEMON_ENTER, EventArgs(newPoke, nullptr));
    if (m_Turn[1].move != &MOVE_SWITCH && m_Turn[1].target == currentPoke) m_Turn[1].target = newPoke;
    newPokePosition = -1;
    setPokemonHandlerOrder();
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

void Battle::raiseEvent(Event event, const EventArgs& args){
    assert(event != PRIORITY_END_OF_TURN, "Don't call raiseEvent with PRIORITY_END_OF_TURN.");
    bool fasterPokemonIsPlayer1 = m_FasterPokemon == player1ActivePokemon;
    auto& fasterPokemonFieldEffects = fasterPokemonIsPlayer1 ? m_Player1FieldEffects : m_Player2FieldEffects;
    auto& slowerPokemonFieldEffects = fasterPokemonIsPlayer1 ? m_Player2FieldEffects : m_Player1FieldEffects;

    if (weather != WEATHER_NONE && weatherSuppressors <= 0) weather->handleEvent(event, nullptr, this, args);
    if (weather != WEATHER_NONE && event == END_OF_TURN) weather->handleEvent(PRIORITY_END_OF_TURN, nullptr, this, args);
    killTheDead();

    m_FasterPokemon->handleEvent(event, args);
    killTheDead();
    for (auto [effect, effectState] : fasterPokemonFieldEffects){
        if (!effectState.suppressed) effect->handleEvent(event, nullptr, this, args);
        if (event == END_OF_TURN) effect->handleEvent(PRIORITY_END_OF_TURN, nullptr, this, args);
        killTheDead();
    }
    removeMarkedFieldEffects(fasterPokemonIsPlayer1);


    m_SlowerPokemon->handleEvent(event, args);
    killTheDead();
    for (auto [effect, effectState]: slowerPokemonFieldEffects){
        if (!effectState.suppressed) effect->handleEvent(event, nullptr, this, args);
        if (event == END_OF_TURN) effect->handleEvent(PRIORITY_END_OF_TURN, nullptr, this, args);
        killTheDead();
    }
    removeMarkedFieldEffects(!fasterPokemonIsPlayer1);

    switch (event)
    {
    case AFTER_MOVE:
        for (int i = 0; i < 4; i++){
            if (args.moveUse->move->maxPP > 0 && args.moveUse->user->currentMoves[i] == args.moveUse->move && args.moveUse->usesPP){
                args.moveUse->user->currentPP[i] -= args.moveUse->ppUsage;
            }
        }
        break;
    case END_OF_TURN:
    {
        isTurnOver = true;
        moveNumber = 0;
        turns++;

        //Check if the battle is over
        bool player1Dead = trainerBlackedOut(IS_PLAYER_ONE);
        bool player2Dead = trainerBlackedOut(IS_PLAYER_TWO);
        if (player1Dead || player2Dead) {
            isBattleOver = true;
            if (player1Dead && player2Dead) {
                isDraw = true;
                log("It's a draw!");
            }
            else {
                winner = player1Dead ? m_Player2 : m_Player1;
                log("The winner is " + winner->getFullName() + "!");
            }
        }
        if (!isBattleOver) {
            if (player1ActivePokemon->isDead) player1Switching = m_Player1->pickPokemon(player1ActivePokemon, player2ActivePokemon, this);
            if (player2ActivePokemon->isDead) player2Switching = m_Player2->pickPokemon(player2ActivePokemon, player1ActivePokemon, this);
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
    log(pokemon->nickname + " fainted!");
    raiseEvent(POKEMON_DEATH, EventArgs(pokemon, nullptr));
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
    if (player1ActivePokemon->getStat(SPEED) == player2ActivePokemon->getStat(SPEED)){
        int num = randInt(0,2);
        m_FasterPokemon = (num == 0) ? player1ActivePokemon : player2ActivePokemon;
        m_SlowerPokemon = (num == 0) ? player2ActivePokemon : player1ActivePokemon;
    }
    else{
        bool poke1IsFaster = player1ActivePokemon->getStat(SPEED) > player2ActivePokemon->getStat(SPEED);
        m_FasterPokemon = poke1IsFaster ? player1ActivePokemon : player2ActivePokemon;
        m_SlowerPokemon = poke1IsFaster ? player2ActivePokemon : player1ActivePokemon;
    }
}

void Battle::setActivePokemon(bool isPlayer1, Pokemon* newPokemon){
    if (isPlayer1){
        player1ActivePokemon = newPokemon;
        log(m_Player1->getFullName() + " sent out " + newPokemon->nickname + "!");
    }
    else{
        player2ActivePokemon = newPokemon;
        log(m_Player2->getFullName() + " sent out " + newPokemon->nickname + "!");
    }
}

void Battle::logCurrentStatus(){
    if (doLogging)
    battleLog += m_Player1->getFullName() + ": " + player1ActivePokemon->nickname + " (" + std::to_string(player1ActivePokemon->currentHealth) + "/" + std::to_string(player1ActivePokemon->getStatRaw(HP)) + ")\n" +
                 m_Player2->getFullName() + ": " + player2ActivePokemon->nickname + " (" + std::to_string(player2ActivePokemon->currentHealth) + "/" + std::to_string(player2ActivePokemon->getStatRaw(HP)) + ")\n";
}

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
                    battle->raiseEvent(END_OF_TURN, EventArgs(nullptr, nullptr));
                    battle->switchIfNecessary();
                    battle->logCurrentStatus();
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