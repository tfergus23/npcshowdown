#include "battle/Battle.hpp"
#include <random>




int Battle::randInt(int min, int max){
    int rand = m_Distribution(m_Generator);
    int diff = max - min;
    int offset = rand % diff;
    return min + offset;
}