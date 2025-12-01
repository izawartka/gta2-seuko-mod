#include "utils.h"

namespace Game {
	const std::vector<CAR_MODEL4>& Utils::GetSpawnableCarModels() {
		static constexpr int32_t ids[] = {
			0,1,2,3,4,5,7,8,10,11,
			12,13,14,84,16,17,18,19,21,22,
			23,24,25,26,85,27,28,29,30,31,
			86,32,33,34,35,36,37,38,39,40,
			41,42,44,45,46,47,48,49,50,51,
			52,53,54,55,56,57,58,62,63,64,
			65,66,67,68,69,70,71,72,73,74,
			75,76,77,78,79,80,81,82,83
		};
		static const std::vector<CAR_MODEL4> models(
			reinterpret_cast<const CAR_MODEL4*>(std::begin(ids)),
			reinterpret_cast<const CAR_MODEL4*>(std::end(ids))
		);
		return models;
	}

	const std::vector<short>& Utils::GetAvailableCarRemaps() {
		static constexpr short ids[] = {
			-1,0,1,2,3,4,5,6,7,8,9,
			10,11,12,13,14,15,16,17,18,19,
			20,21,22,23,24,25,26,27,28,29,
			30,31,32,33,34,35,36
		};
		static const std::vector<short> remaps(std::begin(ids), std::end(ids));
		return remaps;
	}

	const std::vector<std::tuple<short, PALETTE_BASE>>& Utils::GetAvailableCarRemapsWithPalette() {
		static std::vector<std::tuple<short, PALETTE_BASE>> remaps;
		if (!remaps.empty()) return remaps;

		remaps.emplace_back(0, PALETTE_BASE::PALETTE_BASE_SPRITE);
		for (short i = 0; i <= 36; ++i) {
			remaps.emplace_back(i, PALETTE_BASE::PALETTE_BASE_CAR_REMAP);
		}
		return remaps;
	}

	const std::vector<WEAPON_INDEX>& Utils::GetAvailableWeapons() {
		static constexpr int32_t ids[] = {
			0,1,2,3,4,5,7,8,9,10,
			11,12,13,14,15,16,17,18,19,20,
			21,22,23,24,25,26,27
		};
		static const std::vector<WEAPON_INDEX> weapons(
			reinterpret_cast<const WEAPON_INDEX*>(std::begin(ids)),
			reinterpret_cast<const WEAPON_INDEX*>(std::end(ids))
		);
		return weapons;
	}

	bool Utils::IsWeaponVehicleWeapon(WEAPON_INDEX weapon) {
		return static_cast<int>(weapon) >= 15;
	}

	const std::vector<EXPLOSION_SIZE>& Utils::GetAvailableExplosionSizes() {
		static const std::vector<EXPLOSION_SIZE> explosionSizes({
			EXPLOSION_SIZE_NONE,
			EXPLOSION_SIZE_SMALL,
			EXPLOSION_SIZE_MEDIUM,
			EXPLOSION_SIZE_LARGE
		});
		return explosionSizes;
	}
}
