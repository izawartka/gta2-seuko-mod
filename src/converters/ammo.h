#pragma once
#include "default.h"
#include "../game/game.h"

class AmmoConverter {
public:
	static std::wstring ConvertToString(short value) {
		if(value <= -1) {
			return L"-1";
		}

		float floatValue = static_cast<float>(value) / 10.0f;
		return DefaultConverter<float>::ConvertToString(floatValue);
	}

	static short ConvertFromString(std::wstring text) {
		if (text[0] == L'-') return -1;

		float floatValue = DefaultConverter<float>::ConvertFromString(text);
		return static_cast<short>(floatValue * 10.0f);
	}

	static bool IsValidChar(std::wstring text, wchar_t c) {
		return DefaultConverter<float>::IsValidChar(text, c);
	}

	static bool AreEqual(short a, short b) {
		return a == b;
	}

protected:
	AmmoConverter() = delete;
	~AmmoConverter() = delete;
	AmmoConverter(const AmmoConverter&) = delete;
	AmmoConverter& operator=(const AmmoConverter&) = delete;
};
