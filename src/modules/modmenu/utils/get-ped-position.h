#pragma once
#include "../common.h"

namespace ModMenuModule::Utils {
	// if you're sure you're working with the player's main ped, you may want to use PlayerPosCheat instead
	Game::SCR_Vector3* GetPedPosition(Game::Ped* ped);
}
