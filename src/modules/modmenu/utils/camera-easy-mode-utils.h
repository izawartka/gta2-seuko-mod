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
	bool SetCurrentMode(CameraEasyMode newMode);
	const std::vector<CameraEasyMode>& GetSelectableModes();
}
