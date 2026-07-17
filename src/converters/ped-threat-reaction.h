#pragma once
#include <string>
#include "../game/game.h"

class PedThreatReactionConverter {
public:
	static std::wstring ConvertToString(Game::PED_THREAT_REACTION value) {
		switch (value) {
		case Game::PED_THREAT_REACTION_NO_REACTION: return L"No reaction";
		case Game::PED_THREAT_REACTION_REACT_AS_EMERGENCY: return L"React as emergency";
		case Game::PED_THREAT_REACTION_REACT_AS_NORMAL: return L"React as normal";
		case Game::PED_THREAT_REACTION_RUN_AWAY: return L"Run away";
		default:
			return L"Unknown " + std::to_wstring(static_cast<int32_t>(value));
		}
	}

	static bool AreEqual(Game::PED_THREAT_REACTION a, Game::PED_THREAT_REACTION b) {
		return a == b;
	}

protected:
	PedThreatReactionConverter() = delete;
	~PedThreatReactionConverter() = delete;
	PedThreatReactionConverter(const PedThreatReactionConverter&) = delete;
	PedThreatReactionConverter& operator=(const PedThreatReactionConverter&) = delete;
};
