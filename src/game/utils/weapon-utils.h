#pragma once
#include "../gta2_wrapper.h"

namespace Game::Utils {
	constexpr bool IsWeaponVehicleWeapon(WEAPON_INDEX weapon) {
		return static_cast<int>(weapon) >= 15;
	}
}
