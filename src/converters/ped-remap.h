#pragma once
#include <string>
#include "../game/game.h"

class PedRemapConverter {
public:
	static std::wstring ConvertToString(Game::PED_REMAP value) {
		int32_t id = static_cast<int32_t>(value);

		switch (id) {
			case 0: return L"Blue Police";
			case 1: return L"Green Police";
			case 2: return L"Red Police";
			case 3: return L"Yellow Police";
			case 4: return L"Khaki Police";
			case 5: return L"Red Head Redneck";
			case 6: return L"Blond Head Redneck";
			case 7: return L"Yellow Scientist";
			case 8: return L"Zaibatsu";
			case 9: return L"Kristna";
			case 10: return L"Russian";
			case 11: return L"Loony";
			case 12: return L"Elvis";
			case 13: return L"Yakuza";
			case 14: return L"Fireman";
			case 15: return L"Dummy 15";
			case 16: return L"Medic";
			case 17: return L"Mugger";
			case 18: return L"Dummy 18";
			case 19: return L"Dummy 19";
			case 20: return L"Dummy 20";
			case 21: return L"Dummy 21";
			case 22: return L"Prison Uniform";
			case 23: return L"Hulk1";
			case 24: return L"Hulk2";
			case 25: return L"Player";
			case 26: return L"Naked";
			case 27: return L"Dummy 27";
			case 28: return L"Dummy 28";
			case 29: return L"Dummy 29";
			case 30: return L"Dummy 30";
			case 31: return L"Dummy 31";
			case 32: return L"Dummy 32";
			case 33: return L"Dummy 33";
			case 34: return L"Dummy 34";
			case 35: return L"Dummy 35";
			case 36: return L"Dummy 36";
			case 37: return L"Dummy 37";
			case 38: return L"Dummy 38";
			case 39: return L"Dummy 39";
			case 40: return L"Dummy 40";
			case 41: return L"Dummy 41";
			case 42: return L"Dummy 42";
			case 43: return L"Dummy 43";
			case 44: return L"Dummy 44";
			case 45: return L"Dummy 45";
			case 46: return L"Dummy 46";
			case 47: return L"Dummy 47";
			case 48: return L"Dummy 48";
			case 49: return L"Dummy 49";
			case 50: return L"Dummy 50";
			case 51: return L"Dummy 51";
			case 52: return L"Dummy 52";
		default:
			return L"Unknown " + std::to_wstring(id);
		}

	}

	static bool AreEqual(Game::PED_REMAP a, Game::PED_REMAP b) {
		return a == b;
	}

protected:
	PedRemapConverter() = delete;
	~PedRemapConverter() = delete;
	PedRemapConverter(const PedRemapConverter&) = delete;
	PedRemapConverter& operator=(const PedRemapConverter&) = delete;
};
