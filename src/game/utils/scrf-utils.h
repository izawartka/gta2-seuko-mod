#pragma once
#include "../gta2_wrapper.h"

namespace Game::Utils {
	constexpr float ToFloat(SCR_f value) {
		return static_cast<float>(value) / 16384.0f;
	}

	constexpr SCR_f FromFloat(float value) {
		return static_cast<SCR_f>(value * 16384.0f);
	}

	constexpr SCR_f Multiply(SCR_f a, SCR_f b) {
		return static_cast<SCR_f>((static_cast<int64_t>(a) * static_cast<int64_t>(b)) >> 14);
	}
}
