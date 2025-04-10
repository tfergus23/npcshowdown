#include "sim/battle/Battle.hpp"
#include <random>
#include "sim/data/Moves.hpp"
#include "sim/data/Weathers.hpp"
#include "sim/data/Items.hpp"
#include <iostream>

Battle::Battle(const Trainer& trainer1, const Trainer& trainer2, size_t seed, const DebugOptions& debugOptions) :
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
    log("Seed: " + std::to_string(seed));
    setActivePokemon(IS_PLAYER_ONE, &(player1Team[0]));
    setActivePokemon(IS_PLAYER_TWO, &(player2Team[0]));
    setPokemonHandlerOrder();
    raiseEvent(Event::POKEMON_ENTER, EventArgs(m_FasterPokemon, nullptr));
    raiseEvent(Event::POKEMON_ENTER, EventArgs(m_SlowerPokemon, nullptr));
}

const TrainerInfo* Battle::getPlayer1() const {
    return &m_Player1;
}
const TrainerInfo* Battle::getPlayer2() const {
    return &m_Player2;
}

void Battle::log(std::string_view message){
    if (doLogging){
        battleLog += message;
        battleLog.push_back('\n');
    }
    //std::cout << message << '\n';
}

void Battle::debug(std::string_view message){
    if (doLogging && debugOptions.debugLogging){
        battleLog += message;
        battleLog.push_back('\n');
    }
}

void Battle::assertTrue(bool condition, std::string_view message){
    if (!condition){
        log("Assertion failed! Stopping battle.");
        std::cerr << message << '\n';
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
    MoveUse& moveUse = isPlayer1 ? m_Turn[0] : m_Turn[1];
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
    setActivePokemon(isPlayer1, newPoke);
    raiseEvent(Event::POKEMON_ENTER, EventArgs(newPoke, nullptr));
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
    assertTrue(event != Event::PRIORITY_END_OF_TURN, "Don't call raiseEvent with PRIORITY_END_OF_TURN.");
    bool fasterPokemonIsPlayer1 = m_FasterPokemon == player1ActivePokemon;
    auto& fasterPokemonFieldEffects = fasterPokemonIsPlayer1 ? m_Player1FieldEffects : m_Player2FieldEffects;
    auto& slowerPokemonFieldEffects = fasterPokemonIsPlayer1 ? m_Player2FieldEffects : m_Player1FieldEffects;

    if (weather != WEATHER_NONE && weatherSuppressors <= 0) weather->observer.handleEvent(event, nullptr, this, args);
    if (weather != WEATHER_NONE && event == Event::END_OF_TURN) weather->observer.handleEvent(Event::PRIORITY_END_OF_TURN, nullptr, this, args);
    killTheDead();

    m_FasterPokemon->handleEvent(event, args);
    killTheDead();
    for (auto [effect, effectState] : fasterPokemonFieldEffects){
        if (!effectState.suppressed) effect->observer.handleEvent(event, nullptr, this, args);
        if (event == Event::END_OF_TURN) effect->observer.handleEvent(Event::PRIORITY_END_OF_TURN, nullptr, this, args);
        killTheDead();
    }
    removeMarkedFieldEffects(fasterPokemonIsPlayer1);


    m_SlowerPokemon->handleEvent(event, args);
    killTheDead();
    for (auto [effect, effectState]: slowerPokemonFieldEffects){
        if (!effectState.suppressed) effect->observer.handleEvent(event, nullptr, this, args);
        if (event == Event::END_OF_TURN) effect->observer.handleEvent(Event::PRIORITY_END_OF_TURN, nullptr, this, args);
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
                winner = player1Dead ? &m_Player2 : &m_Player1;
                log("The winner is " + winner->name + "!");
            }
        }
        if (turns > 200 && !isBattleOver){
            isBattleOver = true;
            isDraw = true;
            log("It's a draw!");
        }
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
    log(pokemon->nickname + " fainted!");
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

void Battle::setActivePokemon(bool isPlayer1, Pokemon* newPokemon){
    if (isPlayer1){
        player1ActivePokemon = newPokemon;
        log(m_Player1.name + " sent out " + newPokemon->nickname + "!");
    }
    else{
        player2ActivePokemon = newPokemon;
        log(m_Player2.name + " sent out " + newPokemon->nickname + "!");
    }
}

void Battle::logCurrentStatus(){
#if DEBUG_LOG
    std::string statInfo = "";
    Pokemon* pokemon = player1ActivePokemon;
    statInfo += pokemon->nickname + " HP: " + std::to_string(pokemon->getStat(HP)) + " Attack: " + std::to_string(pokemon->getStat(ATTACK)) + " Defense: " + std::to_string(pokemon->getStat(DEFENSE)) + " Special Attack: " + std::to_string(pokemon->getStat(SPATTACK)) + " Special Defense: " + std::to_string(pokemon->getStat(SPDEFENSE)) + " Speed: " + std::to_string(pokemon->getStat(SPEED)) + '\n';
    pokemon = player2ActivePokemon;
    statInfo += pokemon->nickname + " HP: " + std::to_string(pokemon->getStat(HP)) + " Attack: " + std::to_string(pokemon->getStat(ATTACK)) + " Defense: " + std::to_string(pokemon->getStat(DEFENSE)) + " Special Attack: " + std::to_string(pokemon->getStat(SPATTACK)) + " Special Defense: " + std::to_string(pokemon->getStat(SPDEFENSE)) + " Speed: " + std::to_string(pokemon->getStat(SPEED));
    if (doLogging) battleLog += statInfo + '\n';
#endif
    if (doLogging)
    battleLog += m_Player1.name + ": " + player1ActivePokemon->nickname + " (" + std::to_string(player1ActivePokemon->currentHealth) + "/" + std::to_string(player1ActivePokemon->getStatRaw(Stat::HP)) + ")\n" +
                 m_Player2.name + ": " + player2ActivePokemon->nickname + " (" + std::to_string(player2ActivePokemon->currentHealth) + "/" + std::to_string(player2ActivePokemon->getStatRaw(Stat::HP)) + ")\n";
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
                    this->switchIfNecessary();
                    this->logCurrentStatus();
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
        this->winner = this->getPlayer1(); //This could probably remain nullptr, but I'll keep it this for now
        this->log(e.what());
    }
}

const char* BattleAssertionFailedException::what() const noexcept {
    return m_Message.c_str();
}
