#pragma once
#include "../common.h"

namespace ModMenuModule::Utils {
	Game::Ped* GetNearestPed(Game::SCR_Vector3 basePosition, Game::Ped* skipPed = nullptr);
}
