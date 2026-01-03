#pragma once
#include "../common.h"
#include "../gta2_wrapper.h"

namespace Game::Utils {
	const std::vector<CAR_MODEL4>& GetSpawnableCarModels();
	const std::vector<short>& GetAvailableCarRemaps();
	const std::vector<std::tuple<short, PALETTE_BASE>>& GetAvailableCarRemapsWithPalette();
	const std::vector<WEAPON_INDEX>& GetAvailableWeapons();
	const std::vector<EXPLOSION_SIZE>& GetAvailableExplosionSizes();
	const std::vector<PED_REMAP>& GetAvailablePedRemaps();
	const std::vector<PED_GRAPHIC_TYPE>& GetAvailablePedGraphicTypes();
}
