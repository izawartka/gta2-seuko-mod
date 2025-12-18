#pragma once
#include <string>
#include "../modules/modmenu/utils/camera-easy-mode-utils.h"

class CameraEasyModeConverter {
public:
	static std::wstring ConvertToString(ModMenuModule::Utils::CameraEasyMode::CameraEasyMode value) {

		switch (value) {
		case ModMenuModule::Utils::CameraEasyMode::CameraEasyMode::Unmodified:
			return L"Unmodified";
		case ModMenuModule::Utils::CameraEasyMode::CameraEasyMode::FixShadows:
			return L"Fix Shadow distances";
		case ModMenuModule::Utils::CameraEasyMode::CameraEasyMode::Rotate:
			return L"Rotate";
		case ModMenuModule::Utils::CameraEasyMode::CameraEasyMode::ThreeDimensional:
			return L"3D";
		case ModMenuModule::Utils::CameraEasyMode::CameraEasyMode::Custom:
			return L"Custom";
		default: return L"???";
		}
	}

	static bool AreEqual(ModMenuModule::Utils::CameraEasyMode::CameraEasyMode a, ModMenuModule::Utils::CameraEasyMode::CameraEasyMode b) {
		return a == b;
	}

protected:
	CameraEasyModeConverter() = delete;
	~CameraEasyModeConverter() = delete;
	CameraEasyModeConverter(const CameraEasyModeConverter&) = delete;
	CameraEasyModeConverter& operator=(const CameraEasyModeConverter&) = delete;
};
