#pragma once
#include "gta2_wrapper.h"

namespace Game {
	class Utils
	{
	public:
		static float ToFloat(SCR_f value) {
			return static_cast<float>(value) / 16384.0f;
		}

		static SCR_f FromFloat(float value) {
			return static_cast<SCR_f>(value * 16384.0f);
		}
	};
}
