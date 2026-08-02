#pragma once
#include <string>
#include "../game/game.h"

class PedStateConverter {
public:
	static std::wstring ConvertToString(Game::PED_STATE value) {
		switch (value) {
		case Game::PED_STATE_WALKING: return L"Walking";
		// ...
		case Game::PED_STATE_GOING_TO_TARGET: return L"Going to target";
		// ...
		case Game::PED_STATE_LEAVING_CAR: return L"Leaving car";
		// ...
		case Game::PED_STATE_STAYING: return L"Staying";
		case Game::PED_STATE_FALLEN_ON_THE_GROUND: return L"Fallen on the ground";
		case Game::PED_STATE_WASTED: return L"Wasted";
		case Game::PED_STATE_DRIVING_CAR: return L"Driving car";
		case Game::PED_STATE_INITIAL: return L"Initial";
		// ...
		case Game::PED_STATE_SHOT_ON_THE_GROUND: return L"Shot on the ground";
		// ...

		default:
			return L"Unknown " + std::to_wstring(static_cast<int32_t>(value));
		}
	}

	static bool AreEqual(Game::PED_STATE a, Game::PED_STATE b) {
		return a == b;
	}

protected:
	PedStateConverter() = delete;
	~PedStateConverter() = delete;
	PedStateConverter(const PedStateConverter&) = delete;
	PedStateConverter& operator=(const PedStateConverter&) = delete;
};
