#pragma once
#include "sim/battle/Weather.hpp"

inline const Weather* WEATHER_NONE = nullptr;

//TODO
class Sun : public Weather {
	Sun() {

	}
};

inline const Sun WEATHER_SUN;

