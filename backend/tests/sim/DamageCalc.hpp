#include "sim/battle/Battle.hpp"
#include "sim/data/Moves.hpp"
#include "gtest/gtest.h"

TEST(DamageCalcTest, StandardSpecialAttack){
    PokemonBlueprint blastoise(
        "Blastoise",
        100,
        {"Surf","","",""},
        "Torrent",
        "Male",
        {31,31,31,31,31,31},
        "Modest",
        "",
        {252,0,0,252,0,0},
        ""
    );
    PokemonBlueprint snorlax(
        "Snorlax",
        100,
        {"Tackle","","",""},
        "Guts",
        "Male",
        {31,31,31,31,31,31},
        "Careful",
        "",
        {252,0,152,0,104,0},
        ""
    );
    Trainer t1("test1", {blastoise}, TrainerLevel::WILD);
    Trainer t2("test2", {snorlax}, TrainerLevel::WILD);

    constexpr int EXPECTED_DAMAGE = 101;

    Battle battle(t1,t2,0, {.averageDamage = true});
    battle.addMoves(&MOVE_SURF, &MOVE_TACKLE);
    battle.doMove();
    int damage = battle.m_Turn[0].damageDone;

    EXPECT_EQ(damage, EXPECTED_DAMAGE);
}

TEST(DamageCalcTest, StandardPhysicalAttack){

}

TEST(DamageCalcTest, HitsDefSpecialAttack){
    
}

TEST(DamageCalcTest, HitsSPDefPhysicalAttack){
    
}