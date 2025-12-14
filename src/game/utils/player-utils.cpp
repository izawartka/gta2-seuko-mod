#include "player-utils.h"
#include "../memory.h"

short* Game::Utils::GetPlayerPedRotationPtr() {
	Ped* playerPed = Memory::GetPlayerPed();
	if (playerPed == nullptr) return nullptr;

	if (playerPed->gameObject != nullptr) {
		return &playerPed->gameObject->spriteRotation;
	}

	if (playerPed->currentCar != nullptr && playerPed->currentCar->sprite != nullptr) {
		return reinterpret_cast<short*>(&playerPed->currentCar->sprite->rotation);
	}

	return nullptr;
}
