#pragma once
#include "../common.h"

namespace ModMenuModule::Utils::CameraEasyMode {
	enum class CameraEasyMode {
		Unmodified = 0,
		FixShadows,
		Rotate,
		ThreeDimensional,
		Custom
	};

	CameraEasyMode GetCurrentMode();
	CameraEasyMode GetNextMode(const std::vector<CameraEasyMode>& selectableModes);
	bool SetCurrentMode(CameraEasyMode newMode);
	const std::vector<CameraEasyMode>& GetSelectableModes();
}
