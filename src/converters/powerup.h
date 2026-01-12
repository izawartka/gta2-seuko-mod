#pragma once
#include <string>
#include "../game/game.h"

class PowerupConverter {
public:
	static std::wstring ConvertToString(Game::POWERUP_TYPE value) {
		int32_t id = static_cast<int32_t>(value);

		switch (id) {
		case 0: return L"Multiplier +1";
		case 1: return L"Life +1";
		case 2: return L"Health";
		case 3: return L"Armor";
		case 4: return L"Get Outta Jail Free Card";
		case 5: return L"Cop Bribe";
		case 6: return L"Invulnerability";
		case 7: return L"Double Damage";
		case 8: return L"Fast Reload";
		case 9: return L"ElectroFingers";
		case 10: return L"Respect!";
		case 11: return L"Invisibility";
		case 12: return L"Instant Gang";
		case 13: return L"Power-up 13";
		case 14: return L"Power-up 14";
		case 15: return L"Power-up 15";
		case 16: return L"Power-up 16";

		default:
			return L"Unknown " + std::to_wstring(id);
		}

	}

	static bool AreEqual(Game::POWERUP_TYPE a, Game::POWERUP_TYPE b) {
		return a == b;
	}

protected:
	PowerupConverter() = delete;
	~PowerupConverter() = delete;
	PowerupConverter(const PowerupConverter&) = delete;
	PowerupConverter& operator=(const PowerupConverter&) = delete;
};
