#pragma once
#include "scrf.h"
#include "gta-angle.h"

class ScrVector3RotConverter {
public:
	static std::wstring ConvertToString(std::pair<Game::SCR_Vector3, short> value) {
		std::wstring x = ScrfConverter::ConvertToString(value.first.x);
		std::wstring y = ScrfConverter::ConvertToString(value.first.y);
		std::wstring z = ScrfConverter::ConvertToString(value.first.z);
		std::wstring rot = GtaAngleConverter<>::ConvertToString(value.second);

		return L"(" + x + L", " + y + L", " + z + L", " + rot + L" deg)";
	}

	static bool AreEqual(std::pair<Game::SCR_Vector3, short> a, std::pair<Game::SCR_Vector3, short> b) {
		return ScrfConverter::AreEqual(a.first.x, b.first.x) &&
			ScrfConverter::AreEqual(a.first.y, b.first.y) &&
			ScrfConverter::AreEqual(a.first.z, b.first.z) &&
			GtaAngleConverter<>::AreEqual(a.second, b.second);
	}

protected:
	ScrVector3RotConverter() = delete;
	~ScrVector3RotConverter() = delete;
	ScrVector3RotConverter(const ScrVector3RotConverter&) = delete;
	ScrVector3RotConverter& operator=(const ScrVector3RotConverter&) = delete;
};
