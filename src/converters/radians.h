#pragma once
#include "float.h"
#define _USE_MATH_DEFINES
#include <math.h>

template<size_t precision = 1, bool normalize = true, bool inDegrees = true>
class RadiansConverter {
public:
	static std::wstring ConvertToString(float value) {
		std::fesetround(FE_TONEAREST);
		if constexpr (normalize) value = NormalizeValue(value);
		if constexpr (inDegrees) value *= (180.0f / static_cast<float>(M_PI));
		return FloatConverter<float, precision>::ConvertToString(value);
	}

	static float ConvertFromString(std::wstring text) {
		std::fesetround(FE_TONEAREST);
		float value = FloatConverter<float, precision>::ConvertFromString(text);
		if constexpr (inDegrees) value *= (static_cast<float>(M_PI) / 180.0f);
		if constexpr (normalize) value = NormalizeValue(value);
		return value;
	}

	static bool IsValidChar(std::wstring text, wchar_t c) {
		return FloatConverter<float, 3>::IsValidChar(text, c);
	}

	static bool AreEqual(float a, float b) {
		return FloatConverter<float, 3>::AreEqual(a, b);
	}

	static float NormalizeValue(float value) {
		std::fesetround(FE_TONEAREST);
		while (value < 0) {
			value += static_cast<float>(2.0 * M_PI);
		}
		while (value >= static_cast<float>(2.0 * M_PI)) {
			value -= static_cast<float>(2.0 * M_PI);
		}
		return value;
	}

protected:
	RadiansConverter() = delete;
	~RadiansConverter() = delete;
	RadiansConverter(const RadiansConverter&) = delete;
	RadiansConverter& operator=(const RadiansConverter&) = delete;
};
