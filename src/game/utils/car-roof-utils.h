#pragma once
#include "../gta2_wrapper.h"

namespace Game::Utils {
	Roof* GetCarRoofWithSprite(Roof* startRoof, uint16_t spriteIndex);
	void AddCarRoofForWeapon(Car* car, WEAPON_INDEX weapon);
	uint16_t GetCarRoofSpriteForModel(CAR_MODEL4 carModel);
	int* GetCarRoofOffsetY(uint16_t roofSpriteId);
}