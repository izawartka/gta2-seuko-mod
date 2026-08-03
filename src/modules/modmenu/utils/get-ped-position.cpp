#include "get-ped-position.h"

Game::SCR_Vector3* ModMenuModule::Utils::GetPedPosition(const Game::Ped* ped)
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

	return &sourceSprite->position;
}
