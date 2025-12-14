#pragma once
#include "../gta2_wrapper.h"

namespace Game::Utils {
	Roof* GetCarRoofWithSprite(Roof* startRoof, uint16_t spriteIndex);
	void AddCarRoofForWeapon(Car* car, WEAPON_INDEX weapon);
}