#pragma once
#include <string>
#include "../game/game.h"

class WeaponConverter {
public:
	static std::wstring ConvertToString(Game::WEAPON_INDEX value) {
		int32_t id = static_cast<int32_t>(value);

		switch (id) {
		case 0: return L"Pistol";
		case 1: return L"S Uzi";
		case 2: return L"Rocket Launcher";
		case 3: return L"Electro Gun";
		case 4: return L"Molotov Cocktail";
		case 5: return L"Grenade";
		case 6: return L"Shotgun";
		case 7: return L"Electro Baton";
		case 8: return L"Flamethrower";
		case 9: return L"Silenced S Uzi";
		case 10: return L"Dual Pistol";
		case 11: return L"Weapon 11";
		case 12: return L"Weapon 12";
		case 13: return L"Weapon 13";
		case 14: return L"Weapon 14";
		case 15: return L"Vehicle Bomb";
		case 16: return L"Vehicle Oil Slick";
		case 17: return L"Vehicle Mine";
		case 18: return L"Vehicle Machine Gun";
		case 19: return L"Vehicle Tank Cannon";
		case 20: return L"Vehicle Water Cannon";
		case 21: return L"Vehicle Flamethrower";
		case 22: return L"Vehicle Jeep Turret";
		case 23: return L"Vehicle Instant Bomb";
		case 24: return L"Vehicle Weapon 24";
		case 25: return L"Vehicle Weapon 25";
		case 26: return L"Vehicle Weapon 26";
		case 27: return L"Vehicle Weapon 27";
		default: return L"???";
		}
	}

	static bool AreEqual(Game::CAR_MODEL4 a, Game::CAR_MODEL4 b) {
		return a == b;
	}

protected:
	WeaponConverter() = delete;
	~WeaponConverter() = delete;
	WeaponConverter(const WeaponConverter&) = delete;
	WeaponConverter& operator=(const WeaponConverter&) = delete;
};
