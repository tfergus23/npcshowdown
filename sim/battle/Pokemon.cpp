#include "Pokemon.hpp"
#include "data/Abilities.hpp"
#include "battle/Gender.hpp"
#include "battle/Nature.hpp"
#include "data/Moves.hpp"
#include <iostream>
#include "utils/stage_multipliers.hpp"
#include "data/Items.hpp"

Pokemon::Pokemon(const PokemonBlueprint* blueprint, Battle* battle) :
species{blueprint->species},
nickname{blueprint->nickname},
level{blueprint->level},
evs{blueprint->evs},
ivs{blueprint->ivs},
baseMoves{{getMove(blueprint->moves[0]),getMove(blueprint->moves[1]),getMove(blueprint->moves[2]),getMove(blueprint->moves[3])}},
nature{natures[blueprint->nature]},
currentMoves{{getMove(blueprint->moves[0]),getMove(blueprint->moves[1]),getMove(blueprint->moves[2]),getMove(blueprint->moves[3])}},
m_BaseAbility{abilities[blueprint->abilityName]},
m_CurrentAbility{abilities[blueprint->abilityName]},
m_CurrentItem{items[blueprint->itemName]},
m_BaseItem{items[blueprint->itemName]},
battle{battle}
{
    //TODO: Set species data
    if (blueprint->empty) return;


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
        m_Gender = genders[blueprint->gender];
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
}
const Item* Pokemon::getCurrentItem(){
    return m_CurrentItem;
}
void Pokemon::setCurrentItem(const Item* item){
    m_CurrentItem = item;
}
const Status* Pokemon::getStatus(){
    return m_Status;
}
void Pokemon::applyStatus(const Status* status){
    //TODO Implement the MoveEffects version here
    m_Status = status;
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
    m_Effects[effect] = EffectState();
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
        std::cerr << "Unhandled stat.\n";
        throw 1;
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
        std::cerr << "Unhandled stat.\n";
        throw 1;
    }
}


void Pokemon::resetBoosts(){
    int size = boosts.size();
    for (int i = 0; i < size; i++){
        boosts[(Stat) i] = 0;
    }
}

void Pokemon::beforeMove(MoveUse* moveUse){
    if (getCurrentItem() != ITEM_NONE) getCurrentItem()->beforeMove(moveUse, this);
    if (getStatus() != STATUS_NONE) getStatus()->beforeMove(moveUse, this);
    if (!abilityState.suppressed) getCurrentAbility()->beforeMove(moveUse, this);
    for (auto effect : m_Effects){
        effect.first->beforeMove(moveUse, this);
    }
    removeMarkedEffects();
}
void Pokemon::afterMove(MoveUse* moveUse){
    Battle* battle = moveUse->battle;
    if (getCurrentItem() != ITEM_NONE) getCurrentItem()->afterMove(moveUse, this);
    battle->killTheDead();
    if (getStatus() != STATUS_NONE) getStatus()->afterMove(moveUse, this);
    battle->killTheDead();
    if (!abilityState.suppressed) getCurrentAbility()->afterMove(moveUse, this);
    battle->killTheDead();
    for (auto effect : m_Effects){
        effect.first->afterMove(moveUse, this);
        battle->killTheDead();
    }
    removeMarkedEffects();
}

void Pokemon::onSwitch(){
    battle->debug(nickname + "'s onSwitch called");
    if (!abilityState.suppressed) m_CurrentAbility->onSubjectSwitch(this);
    if (m_CurrentItem != ITEM_NONE) m_CurrentItem->onSubjectSwitch(this);
    if (m_Status != STATUS_NONE) m_Status->onSubjectSwitch(this);
    //TODO maybe do this differently??
    for (auto effect : m_Effects){
        effect.first->onSubjectSwitch(this);
    }
    removeMarkedEffects();
    m_CurrentAbility = m_BaseAbility;
    resetBoosts();
    choiceLockedMove = -1;
    isTrapped = false;
    lastMoveUsed = nullptr;
    triggeredCritMod = 0;
    lastMoveUsedAgainstMe = nullptr;
    currentType[0] = m_BaseType[0];
    currentType[1] = m_BaseType[1];
    currentMoves = baseMoves; // Does this copy the whole array? IDK
    if (storedPP >= 0 && storedPPIndex >= 0){
        currentPP[storedPPIndex] = storedPP;
        storedPP = -1;
        storedPPIndex = -1;
    }
}
void Pokemon::onEnter(){
    if (!abilityState.suppressed) m_CurrentAbility->onSubjectEnter(this);
    if (m_CurrentItem != ITEM_NONE) m_CurrentItem->onSubjectEnter(this);
    if (m_Status != STATUS_NONE) m_Status->onSubjectEnter(this);
    for (auto effect : m_Effects){
        effect.first->onSubjectEnter(this);
    }
    removeMarkedEffects();
    lastMoveUsed = nullptr;
}
void Pokemon::onDeath(){
    if (!abilityState.suppressed) m_CurrentAbility->onSubjectDeath(this);
    if (m_CurrentItem != ITEM_NONE) m_CurrentItem->onSubjectDeath(this);
    for (auto effect : m_Effects){
        effect.first->onSubjectDeath(this);
    }
    removeMarkedEffects();
    m_Status = STATUS_NONE;
    onSwitch();
}
void Pokemon::onAttack(MoveUse* move){
    if (!abilityState.suppressed) m_CurrentAbility->onSubjectAttack(move);
    if (m_CurrentItem != ITEM_NONE) m_CurrentItem->onSubjectAttack(move);
    for (auto effect : m_Effects){
        effect.first->onSubjectAttack(move);
    }
    removeMarkedEffects();
}
void Pokemon::onAttacked(MoveUse* move){
    if (!abilityState.suppressed) m_CurrentAbility->onSubjectAttacked(move);
    if (m_CurrentItem != ITEM_NONE) m_CurrentItem->onSubjectAttacked(move);
    for (auto effect : m_Effects){
        effect.first->onSubjectAttacked(move);
    }
    removeMarkedEffects();
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