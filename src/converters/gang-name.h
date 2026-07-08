#pragma once
#include <string>
#include "../game/game.h"

class GangNameConverter {
public:
	static std::wstring ConvertToString(Game::GangRespect* value) {
		if (!value) return L"???";
		wchar_t* gangName = Game::Functions::GetGangName(value);
		if (gangName == nullptr) return L"Gang " + std::to_wstring(value->id);
		return std::wstring(gangName);
	}

	static bool AreEqual(Game::GangRespect* a, Game::GangRespect* b) {
		return a == b;
	}

protected:
	GangNameConverter() = delete;
	~GangNameConverter() = delete;
	GangNameConverter(const GangNameConverter&) = delete;
	GangNameConverter& operator=(const GangNameConverter&) = delete;
};
