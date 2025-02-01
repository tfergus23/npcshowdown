#pragma once
#include <string>
#include <array>
#include "sim/battle/Observer.hpp"
#include "sim/battle/Ability.hpp"
#include "sim/battle/Item.hpp"
#include "sim/battle/Gender.hpp"
#include "sim/battle/Nature.hpp"
#include "sim/battle/Status.hpp"
#include "sim/battle/Effect.hpp"
#include "sim/data/Species.hpp"
#include "sim/data/Items.hpp"
#include "sim/data/Statuses.hpp"
#include "sim/battle/EffectState.hpp"
#include <vector>

class Move;
struct EffectState;
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
        const Ability* getBaseAbility();
        const Item* getBaseItem();
        std::array<const Move*,4> baseMoves;
        Gender getGender();
        Nature nature;


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
        std::array<const Move*,4> currentMoves;
        int currentHealth;
        bool isDead = false;
        char boosts[8] = {0};
        int triggeredCritMod = 0;
        int choiceLockedMove = -1;
        int storedPP = -1; //Meant to be used by mimic to store the PP the user had before using Mimic
        int storedPPIndex = -1;
        int currentPP[4];
        const Move* nextMove = nullptr;
        int nextMoveCounter = -1;
        Type currentType[2];
        void entrap();
        void releaseTrap();
        bool isTrapped();
        //int disabledIndex = -1;
        const Move* lastMoveUsed = nullptr;
        bool empty = true;

        Pokemon();
        Pokemon(const PokemonBlueprint* blueprint, Battle* battle);
        int getStat(Stat stat, bool crit = false);
        int getStatRaw(Stat stat);
        void resetBoosts();
        void handleEvent(Event event, const EventArgs& args);
        bool shouldDie();
        bool outOfPP();
        bool isType(Type type);
        void disableMove(const Move* move);
        void enableMove(const Move* move);
        bool isMoveDisabled(const Move* move);
        bool hasAbilityUnsuppressed(const Ability* ability);

    private:
        void onSwitch();
        const Ability* m_BaseAbility;
        const Ability* m_CurrentAbility;
        const Item* m_BaseItem = &ITEM_NONE;
        const Item* m_CurrentItem = &ITEM_NONE;
        const Status* m_Status = STATUS_NONE;
        std::unordered_map<const Effect*,EffectState> m_Effects;
        std::vector<const Effect*> m_EffectsToRemove;
        void removeMarkedEffects();
        Gender m_Gender;
        std::unordered_map<const Move*, int> m_DisabledMoves;
        int8_t m_Trappers = 0;
};