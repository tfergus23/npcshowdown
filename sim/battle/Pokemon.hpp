#pragma once
#include <string>
#include "battle/Ability.hpp"
#include "battle/Item.hpp"
#include "battle/Gender.hpp"
#include "battle/Nature.hpp"
#include "battle/Status.hpp"
#include "battle/Type.hpp"
#include "battle/Stat.hpp"
#include "battle/Battle.hpp"
#include "battle/MoveUse.hpp"
#include "battle/PokemonBlueprint.hpp"
#include "battle/EffectState.hpp"
#include <array>
#include "battle/Effect.hpp"
#include "data/Statuses.hpp"

class Move{
    public:
        Move(int id);
        const int id;
        int getPP();
        void usePP(int ppUsage = 1);
    private:
        int m_PP;
};

class Pokemon{
    public:
        //Blueprint
        const std::string species;
        const std::string nickname;
        const int level;
        const std::array<int,6> evs;
        const std::array<int,6> ivs;
        const Ability* getBaseAbility();
        const Item* getBaseItem();
        const std::array<int,4> baseMoves;
        const Gender gender;
        const Nature nature;


        //Battle state
        const Ability* getCurrentAbility();
        void setCurrentAbility(const Ability* ability);
        EffectState abilityState;
        const Item* getCurrentItem();
        void setCurrentItem(const Item* item);
        EffectState itemState;
        const Status* getStatus();
        void applyStatus(const Status* status);
        EffectState statusState;
        bool hasEffect(const Effect* effect);
        void removeEffect(const Effect* effect);
        void applyEffect(const Effect* effect);
        EffectState* getEffectState(const Effect* effect);
        std::array<Move,4> currentMoves;
        int currentHealth;
        bool isDead = false;
        std::unordered_map<Stat,int> boosts = {
            {HP, 0},
            {ATTACK, 0},
            {DEFENSE, 0},
            {SPATTACK, 0},
            {SPDEFENSE, 0},
            {SPEED, 0},
            {ACCURACY, 0},
            {EVASION, 0}
        };
        int triggeredCritMod = 0;
        int choiceLockedMove = -1;
        int storedPP = -1; //Meant to be used by mimic to store the PP the user had before using Mimic
        int storedPPIndex = -1;
        int currentPP[4];
        int nextMove = -1;
        int nextMoveCounter = -1;
        Type currentType[2];
        bool isTrapped = false;
        int disabledIndex = -1;
        int lastMoveIndex = -1;
        MoveUse* lastMoveUsedAgainstMe = nullptr;
        Battle* battle;

        //Species data
        float getWeight();
        float getPercentMale();
        std::array<Type,2> getBaseType();

        Pokemon(const PokemonBlueprint* blueprint, Battle* battle);
        int getStat(Stat stat, bool crit = false);
        int getStatRaw(Stat stat);
        void resetBoosts();
        void onSwitch();
        void onEnter();
        void onDeath();
        void onAttack(MoveUse* move);
        void onAttacked(MoveUse* move);
        bool shouldDie();
        bool outOfPP();
        bool isType(Type type);





    private:
        const Ability* m_BaseAbility;
        const Ability* m_CurrentAbility;
        const Item* m_BaseItem;
        const Item* m_CurrentItem;
        int m_BaseStats[6];
        const Status* m_Status = STATUS_NONE;
        std::unordered_map<const Effect*,EffectState> m_Effects;
        float m_Weight;
        float m_PercentMale;
        std::array<Type,2> m_BaseType;
};