#pragma once
#include <string>
#include "../game/game.h"

class ExplosionSizeConverter {
public:
	static std::wstring ConvertToString(Game::EXPLOSION_SIZE value) {

		switch (value) {
		case Game::EXPLOSION_SIZE_NONE: return L"None";
		case Game::EXPLOSION_SIZE_SMALL: return L"Small";
		case Game::EXPLOSION_SIZE_MEDIUM: return L"Medium";
		case Game::EXPLOSION_SIZE_LARGE: return L"Large";
		default: return L"???";
		}
	}

	static bool AreEqual(Game::EXPLOSION_SIZE a, Game::EXPLOSION_SIZE b) {
		return a == b;
	}

protected:
	ExplosionSizeConverter() = delete;
	~ExplosionSizeConverter() = delete;
	ExplosionSizeConverter(const ExplosionSizeConverter&) = delete;
	ExplosionSizeConverter& operator=(const ExplosionSizeConverter&) = delete;
};
