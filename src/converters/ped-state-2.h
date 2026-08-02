#pragma once
#include <string>
#include "../game/game.h"

class PedState2Converter {
public:
	static std::wstring ConvertToString(Game::PED_STATE2 value) {
		switch (value) {
		case Game::PED_STATE2_WALKING: return L"Walking";
		case Game::PED_STATE2_ELECTROCUTED_1: return L"Electrocuted 1";
		case Game::PED_STATE2_ELECTROCUTED_2: return L"Electrocuted 2";
		// ...
		case Game::PED_STATE2_FOLLOWING_CAR: return L"Following car";
		// ...
		case Game::PED_STATE2_ENTERING_CAR: return L"Entering car";
		case Game::PED_STATE2_LEAVING_CAR: return L"Leaving car";
		case Game::PED_STATE2_DRIVING: return L"Driving";
		case Game::PED_STATE2_WAITING_FOR_BUS: return L"Waiting for bus";
		// ...
		case Game::PED_STATE2_STAYING: return L"Staying";
		// ...
		case Game::PED_STATE2_SMOKING: return L"Smoking";

		default:
			return L"Unknown " + std::to_wstring(static_cast<int32_t>(value));
		}
	}

	static bool AreEqual(Game::PED_STATE2 a, Game::PED_STATE2 b) {
		return a == b;
	}

protected:
	PedState2Converter() = delete;
	~PedState2Converter() = delete;
	PedState2Converter(const PedState2Converter&) = delete;
	PedState2Converter& operator=(const PedState2Converter&) = delete;
};
