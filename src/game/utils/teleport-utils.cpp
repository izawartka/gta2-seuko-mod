#include "teleport-utils.h"
#include "scrf-utils.h"
#include "../memory.h"
#include "../functions.h"

bool Game::Utils::TeleportPlayer(SCR_f x, SCR_f y)
{
	MapBlocks* mapBlocks = Memory::GetMapBlocks();
	if (!mapBlocks) {
		spdlog::warn("Cannot teleport: MapBlocks is null, cannot find Z coordinate");
		return false;
	}

	SCR_f z = 0;
	Functions::FindMaxZ(mapBlocks, 0, &z, x, y);

	return TeleportPlayer(x, y, z);
}

bool Game::Utils::TeleportPlayer(SCR_f x, SCR_f y, SCR_f z)
{
	Game* game = Memory::GetGame();
	if (!game) {
		spdlog::warn("Cannot teleport: Game instance not found");
		return false;
	}

	Player* player = game->CurrentPlayer;
	if (!player) {
		spdlog::warn("Cannot teleport: Current player not found");
		return false;
	}

	Ped* playerPed = player->ped;
	if (!playerPed) {
		spdlog::warn("Cannot teleport: Player ped not found");
		return false;
	}

	if (playerPed->currentCar) {
		Functions::TeleportCar(playerPed->currentCar, 0, x, y, z);
	}
	else if (playerPed->gameObject) {
		Functions::TeleportGameObject(playerPed->gameObject, 0, x, y, z);
	}
	else {
		spdlog::warn("Cannot teleport: Player ped has no game object");
		return false;
	}

	player->ph1.followState = CAMERA_FOLLOW_STATE_FOLLOW;
	player->teleportMode = 0;
	player->state1 = PLAYER_PHYSICS_STATE_0;

	spdlog::info("Teleported player to ({}, {}, {})", Utils::ToFloat(x), Utils::ToFloat(y), Utils::ToFloat(z));

	return true;
}
