#include "player-utils.h"
#include "../memory.h"
#include "../functions.h"

Game::Player* Game::Utils::GetPlayer()
{
	Game* game = Memory::GetGame();
	if (!game) return nullptr;

	return game->currentPlayer;
}

Game::Ped* Game::Utils::GetPlayerCurrentPed()
{
	Game* game = Memory::GetGame();
	if (!game) return nullptr;
	Player* player = game->currentPlayer;
	if (!player) return nullptr;

	return Functions::GetCurrentPed(player);
}

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

short* Game::Utils::GetPlayerCurrentPedRotationPtr() {
	Ped* playerPed = GetPlayerCurrentPed();
	if (playerPed == nullptr) return nullptr;

	if (playerPed->gameObject != nullptr) {
		return &playerPed->gameObject->spriteRotation;
	}

	if (playerPed->currentCar != nullptr && playerPed->currentCar->sprite != nullptr) {
		return reinterpret_cast<short*>(&playerPed->currentCar->sprite->rotation);
	}

	return nullptr;
}
