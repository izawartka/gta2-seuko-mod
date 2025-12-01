#pragma once
#include "gta2_wrapper.h"
#include "memory.h"
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

namespace Game {
	class Utils
	{
	public:
		Utils() = delete;
		~Utils() = delete;

		static constexpr float ToFloat(SCR_f value) {
			return static_cast<float>(value) / 16384.0f;
		}

		static constexpr SCR_f FromFloat(float value) {
			return static_cast<SCR_f>(value * 16384.0f);
		}

		static constexpr SCR_f Multiply(SCR_f a, SCR_f b) {
			return static_cast<SCR_f>((static_cast<int64_t>(a) * static_cast<int64_t>(b)) >> 14);
		}

		static constexpr float FromGTAAngleToRad(short gtaAngle) {
			return static_cast<float>(gtaAngle) * (static_cast<float>(M_PI) / 720.0f);
		}

		static constexpr short FromRadToGTAAngle(float angleRad) {
			return static_cast<short>(angleRad * (720.0f / static_cast<float>(M_PI)));
		}

		static const std::vector<CAR_MODEL4>& GetSpawnableCarModels();
		static const std::vector<short>& GetAvailableCarRemaps();
		static const std::vector<std::tuple<short, PALETTE_BASE>>& GetAvailableCarRemapsWithPalette();
		static const std::vector<WEAPON_INDEX>& GetAvailableWeapons();
		static bool IsWeaponVehicleWeapon(WEAPON_INDEX weapon);
		static const std::vector<EXPLOSION_SIZE>& GetAvailableExplosionSizes();
		static std::pair<int, int> GetScreenDimensions();
		static short* GetPlayerPedRotationPtr();
	};
}
