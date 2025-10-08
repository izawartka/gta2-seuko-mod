#pragma once
#include "default.h"
#include "../game/game.h"

class ScrfConverter {
public:
	static std::wstring ConvertToString(Game::SCR_f value) {
		float floatValue = Game::Utils::ToFloat(value);
		return DefaultConverter<float>::ConvertToString(floatValue);
	}

	static Game::SCR_f ConvertFromString(std::wstring text) {
		float floatValue = DefaultConverter<float>::ConvertFromString(text);
		return Game::Utils::FromFloat(floatValue);
	}

	static bool IsValidChar(std::wstring text, wchar_t c) {
		return DefaultConverter<float>::IsValidChar(text, c);
	}

protected:
	ScrfConverter() = delete;
	~ScrfConverter() = delete;
	ScrfConverter(const ScrfConverter&) = delete;
	ScrfConverter& operator=(const ScrfConverter&) = delete;
};
