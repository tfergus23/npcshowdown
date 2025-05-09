#include "Battle.hpp"
#include "Pokemon.hpp"
#include "sim/data/Abilities.hpp"
#include "sim/battle/Gender.hpp"
#include "sim/battle/Nature.hpp"
#include "sim/data/Moves.hpp"
#include <iostream>
#include "sim/utils/stage_multipliers.hpp"
#include "sim/data/Items.hpp"
#include "sim/battle//Stat.hpp"
#include "sim/battle/PokemonBlueprint.hpp"

Pokemon::Pokemon() :
level{-1},
evs{std::array<int,6>()},
ivs{std::array<int,6>()},
baseMoves{std::array<const Move*, 4>()},
nature{Nature::ADAMANT}
{
}

Pokemon::Pokemon(const PokemonBlueprint* blueprint, Battle* battle) :
species{speciesFromString(blueprint->species)},
nickname{blueprint->nickname == "" ? blueprint->species : blueprint->nickname},
level{blueprint->level},
baseMoves{{moveFromString(blueprint->moves[0]),moveFromString(blueprint->moves[1]),moveFromString(blueprint->moves[2]),moveFromString(blueprint->moves[3])}},
nature{natureFromString(blueprint->nature)},
currentMoves{{moveFromString(blueprint->moves[0]),moveFromString(blueprint->moves[1]),moveFromString(blueprint->moves[2]),moveFromString(blueprint->moves[3])}},
m_BaseAbility{abilityFromString(blueprint->abilityName)},
m_CurrentAbility{abilityFromString(blueprint->abilityName)},
m_CurrentItem{itemFromString(blueprint->itemName)},
m_BaseItem{itemFromString(blueprint->itemName)},
battle{battle}
{
    for (int i = 0; i < evs.size(); i++){
        evs[i] = blueprint->evs[i];
        ivs[i] = blueprint->ivs[i];
    }
    //TODO: Set species data
    currentType[0] = species->type[0];
    currentType[1] = species->type[1];

    battle->assertTrue(m_CurrentAbility != nullptr, nickname + " doesn't have an ability.");
    m_CurrentAbility->observer.initialize(this, battle);
    if (m_CurrentItem != &ITEM_NONE)
        m_CurrentItem->observer.initialize(this, battle);

    empty = false;
    for (int i = 0; i < 4; i++){
        if (baseMoves[i] != nullptr){
            currentPP[i] = baseMoves[i]->maxPP;
        }
    }
    
    switch ((int) species->percentMale)
    {
    case -1:
        m_Gender = Gender::GENDERLESS;
        break;
    case 0:
        m_Gender = Gender::FEMALE;
        break;
    case 100:
        m_Gender = Gender::MALE;
        break;
    default:
        if (blueprint->gender == "Random"){
            int num = battle->randInt(0,2);
            switch (num)
            {
            case 0:
                m_Gender = Gender::MALE;
                break;
            case 1:
                m_Gender = Gender::FEMALE;
                break;
            default:
                throw std::runtime_error("Pulled an invalid random int when assigning gender.");
            }
        }
        else{
            m_Gender = stringToGender.at(blueprint->gender);
        }
        break;
    }
    currentHealth = getStatRaw(Stat::HP);
}

const Ability* Pokemon::getBaseAbility(){
    return m_BaseAbility;
}
const Item* Pokemon::getBaseItem(){
    return m_BaseItem;
}
const Ability* Pokemon::getCurrentAbility(){
    return m_CurrentAbility;
}
void Pokemon::setCurrentAbility(const Ability* ability){
    m_CurrentAbility = ability;
    m_CurrentAbility->observer.initialize(this, battle);
}
const Item* Pokemon::getCurrentItem(){
    return m_CurrentItem;
}
void Pokemon::setCurrentItem(const Item* item){
    m_CurrentItem = item;
    if (m_CurrentItem != &ITEM_NONE)
        m_CurrentItem->observer.initialize(this, battle);

}
const Status* Pokemon::getStatus(){
    return m_Status;
}
void Pokemon::applyStatus(const Status* status){
    m_Status = status;
    if (m_Status != &STATUS_NONE)
        m_Status->observer.initialize(this, battle);
}
bool Pokemon::hasEffect(const Effect* effect){
    return m_Effects.count(effect) > 0;
}
void Pokemon::removeEffect(const Effect* effect){
    m_EffectsToRemove.push_back(effect);
}
void Pokemon::removeMarkedEffects(){
    for (auto effect : m_EffectsToRemove){
        m_Effects.erase(effect);
    }
    m_EffectsToRemove.clear();
}

void Pokemon::applyEffect(const Effect* effect){
    //TODO Implement the MoveEffects version here
    battle->assertTrue(!hasEffect(effect), "Tried to apply effect " + effect->name +  " to " + nickname + ", but " + nickname + " already has that effect.");
    m_Effects[effect];
    effect->observer.initialize(this, battle);
}
ObserverState* Pokemon::getEffectState(const Effect* effect){
    return &m_Effects[effect];
}

Gender Pokemon::getGender() const{
    return m_Gender;
}

int Pokemon::getStat(Stat stat, bool crit){
    int unboostedStat = getStatRaw(stat);
    int finalStatValue = unboostedStat;
    switch (stat)
    {
    case Stat::HP:
        finalStatValue = unboostedStat;
        break;
    case Stat::ATTACK:
    case Stat::SPATTACK:
    {
        int boost = (crit && boosts[(int)stat] < 0) ? 0 : boosts[(int)stat];
        finalStatValue = (int)floor((float)unboostedStat * statStageMultiplier(boost));
        break;
    }
    case Stat::DEFENSE:
    case Stat::SPDEFENSE:
    {
        int boost = (crit && boosts[(int)stat] > 0) ? 0 : boosts[(int)stat];
        finalStatValue = (int)floor((float)unboostedStat * statStageMultiplier(boost));
        break;
    }
    case Stat::SPEED:
    {
        float paralysisMod = 1.0f;
        if (m_Status == &STATUS_PARALYSIS) paralysisMod = 0.5f;
        finalStatValue = (int)floor((float)unboostedStat * paralysisMod * statStageMultiplier(boosts[(int)stat]));
        break;
    }
    default:
        battle->assertTrue(false, "Unhandled stat: " + std::to_string((int)stat));
    }
    finalStatValue = m_CurrentAbility->modifySubjectStat(stat, finalStatValue, this);
    return finalStatValue;
}
int Pokemon::getStatRaw(Stat stat) const{
    switch (stat)
    {
    case Stat::HP:
        return (int) floor(((2 * species->baseStats[(int)stat] + ivs[(int)stat] + floor(evs[(int)stat] / 4.0f)) * level)/100.0f) + level + 10;
    case Stat::ATTACK:
    case Stat::SPATTACK:
    {
        int div1 = (int)floor(evs[(int)stat] / 4.0f);
        int div2 = (int)floor(((2 * species->baseStats[(int)stat] + ivs[(int)stat] + div1) * level) / 100.0f);
        int unboostedStat = (int)floor((div2 + 5) * natureBoost(nature, stat));
        return unboostedStat;
    }
    case Stat::DEFENSE:
    case Stat::SPDEFENSE:
    {
        int div1 = (int)floor(evs[(int)stat] / 4.0f);
        int div2 = (int)floor(((2 * species->baseStats[(int)stat] + ivs[(int)stat] + div1) * level) / 100.0f);
        int unboostedStat = (int)floor((div2 + 5) * natureBoost(nature, stat));
        return unboostedStat;
    }
    case Stat::SPEED:
    {
        int div1 = (int)floor(evs[(int)stat] / 4.0f);
        int div2 = (int)floor(((2 * species->baseStats[(int)stat] + ivs[(int)stat] + div1) * level) / 100.0f);
        int unboostedStat = (int)floor((div2 + 5) * natureBoost(nature, stat));
        return unboostedStat;
    }
    default:
        battle->assertTrue(false, "Unhandled stat: " + std::to_string((int)stat));
        return -1;
    }
}


void Pokemon::resetBoosts(){
    for (int i = 0; i < 8; i++){
        boosts[i] = 0;
    }
}

void Pokemon::handleEvent(Event event, const EventArgs& args){
    //Still run this if we just died
    if (isDead && !(event == Event::POKEMON_DEATH && args.eventSubject == this)) return;

    if (getCurrentItem() != &ITEM_NONE && !isItemSuppressed()) getCurrentItem()->observer.handleEvent(event,this, battle, args);
    if (getCurrentItem() != &ITEM_NONE && event == Event::END_OF_TURN) getCurrentItem()->observer.handleEvent(Event::PRIORITY_END_OF_TURN,this, battle, args);

    if (getStatus() != &STATUS_NONE && !isStatusSuppressed()) getStatus()->observer.handleEvent(event,this, battle, args);
    if (getStatus() != &STATUS_NONE && event == Event::END_OF_TURN) getStatus()->observer.handleEvent(Event::PRIORITY_END_OF_TURN,this, battle, args);


    if (!isAbilitySuppressed()) getCurrentAbility()->observer.handleEvent(event,this, battle, args);
    if (event == Event::END_OF_TURN) getCurrentAbility()->observer.handleEvent(Event::PRIORITY_END_OF_TURN, this, battle, args);
    for (auto [effect,state] : m_Effects){
        effect->observer.handleEvent(event, this, battle, args);
        if (event == Event::END_OF_TURN) effect->observer.handleEvent(Event::PRIORITY_END_OF_TURN, this, battle, args);
    }
    removeMarkedEffects();

    if (args.eventSubject != this) return;
    switch (event)
    {
    case Event::POKEMON_SWITCH:
        onSwitch();
        break;
    case Event::POKEMON_ENTER:
        lastMoveUsed = nullptr;
        break;
    case Event::POKEMON_DEATH:
        m_Status = &STATUS_NONE;
        onSwitch();
    default:
        break;
    }
}

void Pokemon::onSwitch(){
    setCurrentAbility(m_BaseAbility);
    m_CurrentItem->observer.initialize(this, battle);
    m_Status->observer.initialize(this, battle);
    m_AbilitySuppressors = 0;
    m_ItemSuppressors = 0;
    m_StatusSuppressors = 0;
    resetBoosts();
    choiceLockedMove = -1;
    m_Trappers = 0;
    lastMoveUsed = nullptr;
    triggeredCritMod = 0;
    currentType[0] = species->type[0];
    currentType[1] = species->type[1];
    currentMoves = baseMoves;
    if (storedPP >= 0 && storedPPIndex >= 0){
        currentPP[storedPPIndex] = storedPP;
        storedPP = -1;
        storedPPIndex = -1;
    }
}

bool Pokemon::shouldDie(){
    return currentHealth <= 0 && !isDead;
}
bool Pokemon::outOfPP(){
    for (int i = 0; i < 4; i++){
        int pp = currentPP[i];
        if (pp > 0){
            return false;
        }
    }
    return true;
}
bool Pokemon::isType(Type type){
    return type == currentType[0] || type == currentType[1];
}

void Pokemon::disableMove(const Move* move){
    m_DisabledMoves[move]++;
}
void Pokemon::enableMove(const Move* move){
    battle->assertTrue(m_DisabledMoves[move], "Tried to enable a move that wasn't disabled.");
    m_DisabledMoves[move]--;
}
bool Pokemon::isMoveDisabled(const Move* move){
    return m_DisabledMoves[move];
}

void Pokemon::entrap(){
    m_Trappers++;
}
void Pokemon::releaseTrap(){
    battle->assertTrue(m_Trappers > 0, "Tried to release a pokemon that wasn't trapped.");
    m_Trappers--;
}
bool Pokemon::isTrapped(){
    return m_Trappers;
}

bool Pokemon::hasAbilityUnsuppressed(const Ability* ability){
    return this->m_CurrentAbility == ability && !isAbilitySuppressed();
}

void Pokemon::suppressAbility(){
    m_AbilitySuppressors++;
}
void Pokemon::allowAbility(){
    m_AbilitySuppressors--;
}
bool Pokemon::isAbilitySuppressed(){
    return m_AbilitySuppressors > 0;
}
void Pokemon::suppressItem(){
    m_ItemSuppressors++;
}
void Pokemon::allowItem(){
    m_ItemSuppressors--;
}
bool Pokemon::isItemSuppressed(){
    return m_ItemSuppressors > 0;
}
void Pokemon::suppressStatus(){
    m_StatusSuppressors++;
}
void Pokemon::allowStatus(){
    m_StatusSuppressors--;
}
bool Pokemon::isStatusSuppressed(){
    return m_StatusSuppressors > 0;
}