#pragma once
#include "../common.h"

namespace ModMenuModule::Utils {
	static void GetAllWeapons()
	{
		Game::Ped* playerPed = Game::Memory::GetPlayerPed();
		if (!playerPed) {
			spdlog::warn("Cannot give all weapons: player ped is null");
			return;
		}
		for (int i = 0; i < 15; i++) {
			Game::Functions::AddWeapon(
				playerPed,
				0,
				static_cast<Game::WEAPON_INDEX>(i),
				99
			);
		}
		spdlog::info("All weapons given to player");
	}
}
