#pragma once
#include "../common.h"

namespace ModMenuModule::Utils {
	// if you're sure you're working with the player's main ped, you may want to use PlayerPosCheat instead
	Game::ushort* GetPedRotation(Game::Ped* ped);
}
