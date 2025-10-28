#pragma once
#include <string>
#include <array>
#include "sim/battle/Observer.hpp"
#include "sim/battle/Ability.hpp"
#include "sim/battle/Item.hpp"
#include "sim/battle/Gender.hpp"
#include "sim/battle/Nature.hpp"
#include "sim/battle/Status.hpp"
#include "sim/battle/Volatile.hpp"
#include "sim/data/Species.hpp"
#include "sim/data/Items.hpp"
#include "sim/data/Statuses.hpp"
#include "sim/battle/ObserverState.hpp"
#include <vector>

class Move;
class PokemonBlueprint;

class Pokemon{
    public:
        Battle* battle;
        //Blueprint
        const Species* species;
        std::string nickname;
        int level;
        std::array<int,6> evs;
        std::array<int,6> ivs;
        std::array<const Move*,4> baseMoves;
        Nature nature;


        //Battle state
        ObserverState abilityState;
        ObserverState itemState;
        ObserverState statusState;
        std::array<const Move*,4> currentMoves;
        int currentHealth;
        bool isDead = false;
        int8_t boosts[8] = {0};
        int triggeredCritMod = 0;
        int choiceLockedMove = -1;
        int storedPP = -1; //Meant to be used by mimic to store the PP the user had before using Mimic
        int storedPPIndex = -1;
        int currentPP[4];
        const Move* nextMove = nullptr;
        int nextMoveCounter = -1;
        std::array<Type,2> currentType;
        const Move* lastMoveUsed = nullptr;
        bool empty = true;

        Pokemon();
        Pokemon(const PokemonBlueprint* blueprint, Battle* battle);

        int getStat(Stat stat, bool crit = false);
        int getStatRaw(Stat stat) const;

        void resetBoosts();

        void handleEvent(Event event, const EventArgs& args);

        bool shouldDie();

        bool outOfPP();

        bool isType(Type type);

        void disableMove(const Move* move);
        void enableMove(const Move* move);
        bool isMoveDisabled(const Move* move);

        bool hasAbilityUnsuppressed(const Ability* ability);
        void suppressAbility();
        void allowAbility();
        bool isAbilitySuppressed();

        void suppressItem();
        void allowItem();
        bool isItemSuppressed();

        void suppressStatus();
        void allowStatus();
        bool isStatusSuppressed();

        void entrap();
        void releaseTrap();
        bool isTrapped();

        bool hasVolatile(const Volatile* vol);
        void removeVolatile(const Volatile* vol);
        void applyVolatile(const Volatile* vol);
        ObserverState* getVolatileState(const Volatile* vol);

        const Status* getStatus();
        void applyStatus(const Status* status);

        const Item* getCurrentItem();
        void setCurrentItem(const Item* item);

        const Ability* getCurrentAbility();
        void setCurrentAbility(const Ability* ability);

        Gender getGender() const;

        const Ability* getBaseAbility();
        const Item* getBaseItem();
    private:
        const Ability* m_BaseAbility;
        const Ability* m_CurrentAbility;
        const Item* m_BaseItem = &ITEM_NONE;
        const Item* m_CurrentItem = &ITEM_NONE;
        const Status* m_Status = &STATUS_NONE;
        std::unordered_map<const Volatile*,ObserverState> m_Volatiles;
        std::vector<const Volatile*> m_VolatilesToRemove;
        Gender m_Gender;
        std::unordered_map<const Move*, int> m_DisabledMoves;
        int8_t m_Trappers = 0;
        int8_t m_AbilitySuppressors = 0;
        int8_t m_ItemSuppressors = 0;
        int8_t m_StatusSuppressors = 0;

        void onSwitch();
        void removeMarkedVolatiles();
};