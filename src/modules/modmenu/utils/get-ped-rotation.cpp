#include "get-ped-rotation.h"

Game::ushort* ModMenuModule::Utils::GetPedRotation(Game::Ped* ped)
{
	if (!ped) return nullptr;

	Game::Sprite* sourceSprite = nullptr;
	if (ped->gameObject && ped->gameObject->sprite) {
		sourceSprite = ped->gameObject->sprite;
	}
	else if (ped->currentCar && ped->currentCar->sprite) {
		sourceSprite = ped->currentCar->sprite;
	}
	else {
		return nullptr;
	}

	return &sourceSprite->rotation;
}
