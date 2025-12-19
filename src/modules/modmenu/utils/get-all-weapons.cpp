#include "get-all-weapons.h"

namespace ModMenuModule::Utils {
	bool GetAllWeapons()
	{
		Game::Ped* playerPed = Game::Memory::GetPlayerPed();
		if (!playerPed) {
			spdlog::warn("Cannot give all weapons: player ped is null");
			return false;
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
		return true;
	}
}
