#pragma once
#include "../common.h"

namespace ModMenuModule::Utils {
	struct CarDistanceInfo {
		Game::Car* car;
		Game::SCR_f distanceSquared;
	};

	std::vector<CarDistanceInfo> GetCarsSortedByDistance();
}
