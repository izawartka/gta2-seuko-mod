#pragma once
#include "gta2_wrapper.h"
#include <vector>

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

		static std::vector<CAR_MODEL4> GetSpawnableCarModels() {
			static constexpr int32_t ids[] = {
				0,1,2,3,4,5,7,8,10,11,
				12,13,14,84,16,17,18,19,21,22,
				23,24,25,26,85,27,28,29,30,31,
				86,32,33,34,35,36,37,38,39,40,
				41,42,44,45,46,47,48,49,50,51,
				52,53,54,55,56,57,58,62,63,64,
				65,66,67,68,69,70,71,72,73,74,
				75,76,77,78,79,80,81,82,83
			};
			std::vector<CAR_MODEL4> models;
			models.reserve(std::size(ids));
			for (int32_t id : ids) {
				models.push_back(static_cast<CAR_MODEL4>(id));
			}
			return models;
		}
	};
}
