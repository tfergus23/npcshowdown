#include "Pokemon.hpp"
#include "data/Abilities.hpp"
#include "battle/Gender.hpp"
#include "battle/Nature.hpp"
#include "data/Moves.hpp"
#include <iostream>
#include"utils/stage_multipliers.hpp"

Pokemon::Pokemon(const PokemonBlueprint* blueprint, Battle* battle) :
level{blueprint->level},
evs{blueprint->evs},
ivs{blueprint->ivs},
baseMoves{{getMove(blueprint->moves[0]),getMove(blueprint->moves[1]),getMove(blueprint->moves[2]),getMove(blueprint->moves[3])}},
gender{genders[blueprint->gender]},
nature{natures[blueprint->nature]},
currentMoves{{getMove(blueprint->moves[0]),getMove(blueprint->moves[1]),getMove(blueprint->moves[2]),getMove(blueprint->moves[3])}}
{
    m_BaseAbility = abilities[blueprint->abilityName];
    m_CurrentAbility = abilities[blueprint->abilityName];
    this->battle = battle;
    empty = false;
    //TODO: much more
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
    m_Status = status;
}
bool Pokemon::hasEffect(const Effect* effect){
    return m_Effects.count(effect) > 0;
}
void Pokemon::removeEffect(const Effect* effect){
    m_Effects.erase(effect);
}
void Pokemon::applyEffect(const Effect* effect){
    m_Effects[effect] = EffectState();
}
EffectState* Pokemon::getEffectState(const Effect* effect){
    return &m_Effects[effect];
}
std::array<Type,2> Pokemon::getBaseType(){
    return m_BaseType;
}


int Pokemon::getStat(Stat stat, bool crit = false){
    int finalStatValue;
    switch (stat)
    {
    case HP:
        finalStatValue = (int) floor(((2 * m_BaseStats[stat] + ivs[stat] + floor(evs[stat] / 4.0f)) * level)/100.0f) + level + 10;
        break;
    case ATTACK:
    case SPATTACK:
        int div1 = (int) floor(evs[stat] / 4.0f);
        int div2 = (int) floor(((2 * m_BaseStats[stat] + ivs[stat] + div1) * level) / 100.0f);
        int unboostedStat = (int) floor((div2 + 5) * natureBoost(nature, stat));
        int boost = (crit && boosts[stat] < 0) ? 0 : boosts[stat];
        finalStatValue = (int) floor((float) unboostedStat * statStageMultiplier(boost));
        break;
    case DEFENSE:
    case SPDEFENSE:
    //TODO
        break;
    case SPEED:
    //TODO
        break;
    default:
        std::cerr << "Unhandled stat.\n";
        throw 1;
    }
    finalStatValue = m_CurrentAbility->modifySubjectStat(stat, finalStatValue, this);
    return finalStatValue;
}
int getStatRaw(Stat stat);
void resetBoosts();
void onSwitch();
void onEnter();
void onDeath();
void onAttack(MoveUse &move);
void onAttacked(MoveUse &move);
bool shouldDie();
bool outOfPP();
bool isType(Type type);