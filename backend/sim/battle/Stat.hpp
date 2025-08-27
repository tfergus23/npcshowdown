#pragma once
#include <cstdint>
/*
Order is important here. SPATTACK must be exactly 2 more than ATTACK, 
and SPDEFENSE must be exactly 2 more than DEFENSE
*/
enum class Stat : int8_t {HP, ATTACK, DEFENSE, SPATTACK, SPDEFENSE, SPEED, ACCURACY, EVASION};