#pragma once
#include "float.h"
#include "../game/game.h"

static constexpr size_t PRECISION = 3;

class ScrfConverter {
public:
	static std::wstring ConvertToString(Game::SCR_f value) {
		float floatValue = Game::Utils::ToFloat(value);
		return FloatConverter<float, PRECISION>::ConvertToString(floatValue);
	}

	static Game::SCR_f ConvertFromString(std::wstring text) {
		float floatValue = FloatConverter<float, PRECISION>::ConvertFromString(text);
		return Game::Utils::FromFloat(floatValue);
	}

	static bool IsValidChar(std::wstring text, wchar_t c) {
		return FloatConverter<float, PRECISION>::IsValidChar(text, c);
	}

	static bool AreEqual(Game::SCR_f a, Game::SCR_f b) {
		return a == b;
	}

protected:
	ScrfConverter() = delete;
	~ScrfConverter() = delete;
	ScrfConverter(const ScrfConverter&) = delete;
	ScrfConverter& operator=(const ScrfConverter&) = delete;
};
