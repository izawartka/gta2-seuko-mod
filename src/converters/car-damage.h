#pragma once
#include "float.h"

static constexpr size_t PRECISION = 2;

class CarDamageConverter {
public:
	static std::wstring ConvertToString(short value) {
		float floatValue = value / 320.010f;
		return FloatConverter<float, PRECISION>::ConvertToString(floatValue);
	}

	static short ConvertFromString(std::wstring text) {
		float floatValue = FloatConverter<float, PRECISION>::ConvertFromString(text);
		floatValue = std::clamp(floatValue, 0.0f, 100.0f);
		return static_cast<short>(floatValue * 320.010f);
	}

	static bool IsValidChar(std::wstring text, wchar_t c) {
		if(c == L'-') {
			return false;
		}

		return FloatConverter<float, PRECISION>::IsValidChar(text, c);
	}

	static bool AreEqual(short a, short b) {
		return a == b;
	}

protected:
	CarDamageConverter() = delete;
	~CarDamageConverter() = delete;
	CarDamageConverter(const CarDamageConverter&) = delete;
	CarDamageConverter& operator=(const CarDamageConverter&) = delete;
};
