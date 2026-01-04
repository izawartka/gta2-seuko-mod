#pragma once
#include "../common.h"

namespace ModMenuModule::Utils {
	Game::Car* SpawnCar(Game::SCR_Vector3 position, short rotation, Game::CAR_MODEL4 model, short remap, Game::PALETTE_BASE palette);
}
