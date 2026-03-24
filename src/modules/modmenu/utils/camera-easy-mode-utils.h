#pragma once
#include "../common.h"

namespace ModMenuModule::Utils::CameraEasyMode {
	enum class CameraEasyMode {
		Unmodified = 0,
		Unmodified2 = 1, // Unused, kept for compatibility with old saved data
		Rotate = 2,
		ThreeDimensional = 3,
		Custom = 4
	};

	CameraEasyMode GetCurrentMode();
	CameraEasyMode GetNextMode(const std::vector<CameraEasyMode>& selectableModes);
	bool SetCurrentMode(CameraEasyMode newMode);
	const std::vector<CameraEasyMode>& GetSelectableModes();
}
