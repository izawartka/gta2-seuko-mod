#pragma once
#include "../common.h"
#include "../gta2_wrapper.h"

namespace Game::Utils {
	constexpr float FromGTAAngleToRad(short gtaAngle) {
		return static_cast<float>(gtaAngle) * (static_cast<float>(M_PI) / 720.0f);
	}

	constexpr short FromRadToGTAAngle(float angleRad) {
		return static_cast<short>(angleRad * (720.0f / static_cast<float>(M_PI)));
	}
}
