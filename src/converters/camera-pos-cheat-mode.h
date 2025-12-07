#pragma once
#include <string>
#include "../modules/modmenu/cheats/camera/camera-pos.h"

class CameraPosCheatModeConverter {
public:
	static std::wstring ConvertToString(ModMenuModule::CameraPosCheatMode value) {

		switch (value) {
		case ModMenuModule::CameraPosCheatMode::Unmodified:
			return L"Unmodified";
		case ModMenuModule::CameraPosCheatMode::LockTargetAt:
			return L"Lock at";
		case ModMenuModule::CameraPosCheatMode::IncrementTargetBy:
			return L"Increment by";
		default: return L"???";
		}
	}

	static bool AreEqual(ModMenuModule::CameraPosCheatMode a, ModMenuModule::CameraPosCheatMode b) {
		return a == b;
	}

protected:
	CameraPosCheatModeConverter() = delete;
	~CameraPosCheatModeConverter() = delete;
	CameraPosCheatModeConverter(const CameraPosCheatModeConverter&) = delete;
	CameraPosCheatModeConverter& operator=(const CameraPosCheatModeConverter&) = delete;
};
