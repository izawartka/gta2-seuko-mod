#pragma once
#include "gta2_wrapper.h"
#include "functions.h"

namespace Game {
	class CarRoofUtils {
	public:
		CarRoofUtils() = delete;
		~CarRoofUtils() = delete;

		static Roof* GetCarRoofWithSprite(Roof* startRoof, uint16_t spriteIndex) {
			if (startRoof == nullptr)
				return nullptr;
			if (startRoof->sprite && startRoof->sprite->sprite == spriteIndex)
				return startRoof;
			if (startRoof->next)
				return GetCarRoofWithSprite(startRoof->next, spriteIndex);
			return nullptr;
		}

		static void AddCarRoofForWeapon(Car* car, WEAPON_INDEX weapon) {
			if (car == nullptr) return;

			switch (weapon) {
			case WEAPON_VEHICLE_TANK_CANNON:
				if (GetCarRoofWithSprite(car->roof, 546) != nullptr) return;
				Functions::CarAddRoofTankTurret(car);
				break;
			case WEAPON_VEHICLE_WATER_CANNON:
			case WEAPON_VEHICLE_FLAMETHROWER:
				if (GetCarRoofWithSprite(car->roof, 278) != nullptr) return;
				Functions::CarAddRoofWaterGun(car);
				break;
			case WEAPON_VEHICLE_JEEP_TURRET:
				if (GetCarRoofWithSprite(car->roof, 285) != nullptr) return;
				Functions::CarAddRoofGun(car);
				break;
			}
		}
	};
}