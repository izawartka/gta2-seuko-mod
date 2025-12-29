#include "car-roof-utils.h"
#include "../functions.h"
#include "../memory.h"

Game::Roof* Game::Utils::GetCarRoofWithSprite(Roof* startRoof, uint16_t spriteIndex) 
{
	if (startRoof == nullptr)
		return nullptr;
	if (startRoof->sprite && startRoof->sprite->sprite == spriteIndex)
		return startRoof;
	if (startRoof->next)
		return GetCarRoofWithSprite(startRoof->next, spriteIndex);
	return nullptr;
}

void Game::Utils::AddCarRoofForWeapon(Car* car, WEAPON_INDEX weapon) 
{
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

uint16_t Game::Utils::GetCarRoofSpriteForModel(CAR_MODEL4 carModel) 
{
	switch (carModel) {
	case CAR_MODEL4_TANK:
		return 546;
	case CAR_MODEL4_FIRETRUCK:
		return 278;
	case CAR_MODEL4_GUNJEEP:
		return 285;
	case CAR_MODEL4_TVVAN:
		return 0;
	default:
		return -1; // no special roof
	}
}

int* Game::Utils::GetCarRoofOffsetY(uint16_t roofSpriteId)
{
	switch (roofSpriteId) {
	case 278:
		return Memory::GetWaterCannonRoofOffsetY();
	case 285:
		return Memory::GetJeepGunRoofOffsetY();
	case 546:
		return Memory::GetTankCannonRoofOffsetY();
	case 0:
		return Memory::GetAntennaRoofOffsetY();
	default:
		return nullptr;
	}
}
