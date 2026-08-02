#pragma once
#include "../common.h"

namespace ModMenuModule::Utils {
	Game::Car* GetNearestCar(Game::SCR_Vector3 basePosition, Game::Car* skipCar = nullptr);
}
