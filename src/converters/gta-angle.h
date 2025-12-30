#pragma once
#include "radians.h"
#include "../game/game.h"

template<size_t precision = 1, bool normalize = true, bool inDegrees = true>
class GtaAngleConverter {
public:
	static std::wstring ConvertToString(short value) {
		std::fesetround(FE_TONEAREST);
		float angleRad = Game::Utils::FromGTAAngleToRad(value);
		return RadiansConverter<precision, normalize, inDegrees>::ConvertToString(angleRad);
	}

	static short ConvertFromString(std::wstring text) {
		float angleRad = RadiansConverter<precision, normalize, inDegrees>::ConvertFromString(text);
		return Game::Utils::FromRadToGTAAngle(angleRad);
	}

	static bool IsValidChar(std::wstring text, wchar_t c) {
		return RadiansConverter<precision, normalize, inDegrees>::IsValidChar(text, c);
	}

	static bool AreEqual(float a, float b) {
		return RadiansConverter<precision, normalize, inDegrees>::AreEqual(a, b);
	}

protected:
	GtaAngleConverter() = delete;
	~GtaAngleConverter() = delete;
	GtaAngleConverter(const GtaAngleConverter&) = delete;
	GtaAngleConverter& operator=(const GtaAngleConverter&) = delete;
};
