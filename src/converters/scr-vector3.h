#pragma once
#include "scrf.h"

class ScrVector3Converter {
public:
	static std::wstring ConvertToString(Game::SCR_Vector3 value) {
		std::wstring x = ScrfConverter::ConvertToString(value.x);
		std::wstring y = ScrfConverter::ConvertToString(value.y);
		std::wstring z = ScrfConverter::ConvertToString(value.z);

		return L"(" + x + L", " + y + L", " + z + L")";
	}

	static bool AreEqual(Game::SCR_Vector3 a, Game::SCR_Vector3 b) {
		return a == b;
	}

protected:
	ScrVector3Converter() = delete;
	~ScrVector3Converter() = delete;
	ScrVector3Converter(const ScrVector3Converter&) = delete;
	ScrVector3Converter& operator=(const ScrVector3Converter&) = delete;
};
