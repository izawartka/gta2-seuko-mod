#pragma once
#include <string>
#include "../game/game.h"

class PedThreatSearchConverter {
public:
	static std::wstring ConvertToString(Game::PED_THREAT_SEARCH value) {
		switch (value) {
		case Game::PED_THREAT_SEARCH_NO_THREATS: return L"No threats";
		case Game::PED_THREAT_SEARCH_LINE_OF_SIGHT: return L"Line of sight";
		case Game::PED_THREAT_SEARCH_AREA: return L"Area";
		case Game::PED_THREAT_SEARCH_AREA_PLAYER_THREAT_ONLY: return L"Area; Player threat only";
		case Game::PED_THREAT_SEARCH_LINE_OF_SIGHT_PLAYER_THREAT_ONLY: return L"Line of sight; Player threat only";
		case Game::PED_THREAT_SEARCH_AREA_PLAYER_ONLY: return L"Area; Player only";
		case Game::PED_THREAT_SEARCH_LINE_OF_SIGHT_PLAYER_ONLY: return L"Line of sight; Player only";

		default:
			return L"Unknown " + std::to_wstring(static_cast<int32_t>(value));
		}
	}

	static bool AreEqual(Game::PED_THREAT_SEARCH a, Game::PED_THREAT_SEARCH b) {
		return a == b;
	}

protected:
	PedThreatSearchConverter() = delete;
	~PedThreatSearchConverter() = delete;
	PedThreatSearchConverter(const PedThreatSearchConverter&) = delete;
	PedThreatSearchConverter& operator=(const PedThreatSearchConverter&) = delete;
};
