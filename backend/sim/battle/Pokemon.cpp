#include "Pokemon.hpp"
#include "sim/data/Abilities.hpp"
#include "sim/battle/Gender.hpp"
#include "sim/battle/Nature.hpp"
#include "sim/data/Moves.hpp"
#include <iostream>
#include "sim/utils/stage_multipliers.hpp"
#include "sim/data/Items.hpp"

Pokemon::Pokemon(const PokemonBlueprint* blueprint, Battle* battle) :
species{blueprint->species},
nickname{blueprint->nickname},
level{blueprint->level},
evs{blueprint->evs},
ivs{blueprint->ivs},
baseMoves{{getMove(blueprint->moves[0]),getMove(blueprint->moves[1]),getMove(blueprint->moves[2]),getMove(blueprint->moves[3])}},
nature{natures.at(blueprint->nature)},
currentMoves{{getMove(blueprint->moves[0]),getMove(blueprint->moves[1]),getMove(blueprint->moves[2]),getMove(blueprint->moves[3])}},
m_BaseAbility{abilities.at(blueprint->abilityName)},
m_CurrentAbility{abilities.at(blueprint->abilityName)},
m_CurrentItem{items.at(blueprint->itemName)},
m_BaseItem{items.at(blueprint->itemName)},
battle{battle}
{
    if (blueprint->empty) return;

    //TODO: Set species data

    battle->assert(m_CurrentAbility != nullptr, nickname + " doesn't have an ability.");
    m_CurrentAbility->initializeState(&abilityState);
    if (m_CurrentItem != ITEM_NONE)
        m_CurrentItem->initializeState(&itemState);

    empty = false;
    for (int i = 0; i < 4; i++){
        if (baseMoves[i] != nullptr){
            currentPP[i] = baseMoves[i]->maxPP;
        }
    }
    
    switch (m_PercentMale)
    {
    case -1:
        m_Gender = GENDERLESS;
        break;
    case 0:
        m_Gender = FEMALE;
        break;
    case 100:
        m_Gender = MALE;
        break;
    default:
        m_Gender = genders.at(blueprint->gender);
        break;
    }
    currentHealth = getStatRaw(HP);
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
    abilityState.reset();
    m_CurrentAbility->initializeState(&abilityState);
}
const Item* Pokemon::getCurrentItem(){
    return m_CurrentItem;
}
void Pokemon::setCurrentItem(const Item* item){
    m_CurrentItem = item;
    itemState.reset();
    if (m_CurrentItem != ITEM_NONE)
        m_CurrentItem->initializeState(&itemState);

}
const Status* Pokemon::getStatus(){
    return m_Status;
}
void Pokemon::applyStatus(const Status* status){
    //TODO Implement the MoveEffects version here
    m_Status = status;
    statusState.reset();
    if (m_Status != STATUS_NONE)
        m_Status->initializeState(&statusState);
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
    battle->assert(!hasEffect(effect), "Tried to apply effect " + effect->name +  " to " + nickname + ", but " + nickname + " already has that effect.");
    m_Effects[effect];
    effect->initializeState(&m_Effects[effect]);
}
EffectState* Pokemon::getEffectState(const Effect* effect){
    return &m_Effects[effect];
}
std::array<Type,2> Pokemon::getBaseType(){
    return m_BaseType;
}

Gender Pokemon::getGender(){
    return m_Gender;
}

int Pokemon::getStat(Stat stat, bool crit = false){
    int unboostedStat = getStatRaw(stat);
    int finalStatValue;
    switch (stat)
    {
    case HP:
        finalStatValue = unboostedStat;
        break;
    case ATTACK:
    case SPATTACK:
        int boost = (crit && boosts[stat] < 0) ? 0 : boosts[stat];
        finalStatValue = (int) floor((float) unboostedStat * statStageMultiplier(boost));
        break;
    case DEFENSE:
    case SPDEFENSE:
        int boost = (crit && boosts[stat] > 0) ? 0 : boosts[stat];
        finalStatValue = (int) floor((float)unboostedStat * statStageMultiplier(boost));
        break;
    case SPEED:
        float paralysisMod = 1.0f;
        if (m_Status == STATUS_PARALYSIS) paralysisMod = 0.5f;
        finalStatValue = (int) floor((float)unboostedStat * paralysisMod* statStageMultiplier(boosts[stat]));
        break;
    default:
        battle->assert(false, "Unhandled stat: " + std::to_string(stat));
    }
    finalStatValue = m_CurrentAbility->modifySubjectStat(stat, finalStatValue, this);
    return finalStatValue;
}
int Pokemon::getStatRaw(Stat stat){
    switch (stat)
    {
    case HP:
        return (int) floor(((2 * m_BaseStats[stat] + ivs[stat] + floor(evs[stat] / 4.0f)) * level)/100.0f) + level + 10;
    case ATTACK:
    case SPATTACK:
        int div1 = (int) floor(evs[stat] / 4.0f);
        int div2 = (int) floor(((2 * m_BaseStats[stat] + ivs[stat] + div1) * level) / 100.0f);
        int unboostedStat = (int) floor((div2 + 5) * natureBoost(nature, stat));
        return unboostedStat;
    case DEFENSE:
    case SPDEFENSE:
        int div1 = (int) floor(evs[stat] / 4.0f);
        int div2 = (int) floor(((2 * m_BaseStats[stat] + evs[stat] + div1) * level) / 100.0f);
        int unboostedStat = (int) floor((div2 + 5) * natureBoost(nature,stat));
        return unboostedStat;
    case SPEED:
        int div1 = (int) floor(evs[stat] / 4.0f);
        int div2 = (int) floor(((2 * m_BaseStats[stat] + ivs[stat] + div1) * level) / 100.0f);
        int unboostedStat = (int) floor((div2 + 5) * natureBoost(nature,stat));
        return unboostedStat;
    default:
        battle->assert(false, "Unhandled stat: " + std::to_string(stat));
        return -1;
    }
}


void Pokemon::resetBoosts(){
    int size = boosts.size();
    for (int i = 0; i < size; i++){
        boosts[(Stat) i] = 0;
    }
}

void Pokemon::handleEvent(Event event, const EventArgs& args){
    //TODO: Priotity end of turn somehow....
    if (getCurrentItem() != ITEM_NONE && !itemState.suppressed) getCurrentItem()->handleEvent(event,this, battle, args);
    if (getStatus() != STATUS_NONE && !statusState.suppressed) getStatus()->handleEvent(event,this, battle, args);
    if (!abilityState.suppressed) getCurrentAbility()->handleEvent(event,this, battle, args);
    for (auto [effect,state] : m_Effects){
        if (!state.suppressed) effect->handleEvent(event, this, battle, args);
    }
    removeMarkedEffects();

    switch (event)
    {
    case POKEMON_SWITCH:
        if (args.eventSubject == this){
            onSwitch();
        }
        break;
    case POKEMON_ENTER:
        if (args.eventSubject == this){
            lastMoveUsed = nullptr;
        }
        break;
    case POKEMON_DEATH:
        if (args.eventSubject == this){
            m_Status = STATUS_NONE;
            onSwitch();
        }
    default:
        break;
    }
}

void Pokemon::onSwitch(){
    m_CurrentAbility = m_BaseAbility;
    resetBoosts();
    choiceLockedMove = -1;
    isTrapped = false;
    lastMoveUsed = nullptr;
    triggeredCritMod = 0;
    currentType[0] = m_BaseType[0];
    currentType[1] = m_BaseType[1];
    currentMoves = baseMoves; // Does this copy the whole array? IDK
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