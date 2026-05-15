#pragma once
#include "../gta2_wrapper.h"

namespace Game::Utils {
	Player* GetPlayer();
	Ped* GetPlayerCurrentPed();
	short* GetPlayerPedRotationPtr();
	short* GetPlayerCurrentPedRotationPtr();
}
