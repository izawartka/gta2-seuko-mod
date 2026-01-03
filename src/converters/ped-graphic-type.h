#pragma once
#include <string>
#include "../game/game.h"

class PedGraphicTypeConverter {
public:
	static std::wstring ConvertToString(Game::PED_GRAPHIC_TYPE value) {
		switch (value) {
		case Game::PED_GRAPHIC_TYPE_DUMMY: return L"Dummy";
		case Game::PED_GRAPHIC_TYPE_EMERG: return L"Emergency";
		case Game::PED_GRAPHIC_TYPE_GANG: return L"Gang";
		default:
			return L"Unknown " + std::to_wstring(static_cast<int32_t>(value));
		}
	}

	static bool AreEqual(Game::PED_GRAPHIC_TYPE a, Game::PED_GRAPHIC_TYPE b) {
		return a == b;
	}

protected:
	PedGraphicTypeConverter() = delete;
	~PedGraphicTypeConverter() = delete;
	PedGraphicTypeConverter(const PedGraphicTypeConverter&) = delete;
	PedGraphicTypeConverter& operator=(const PedGraphicTypeConverter&) = delete;
};
