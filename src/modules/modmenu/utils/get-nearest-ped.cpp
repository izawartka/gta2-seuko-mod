#include "get-nearest-ped.h"
#include "get-ped-position.h"

Game::Ped* ModMenuModule::Utils::GetNearestPed(Game::SCR_Vector3 basePosition, Game::Ped* skipPed)
{
	Game::PedManager_S25* pedManager = Game::Memory::GetPedManager();
	if (!pedManager) {
		spdlog::warn("Cannot get nearest ped: PedManager is null");
		return nullptr;
	}

	Game::Ped* currentPed = pedManager->lastPedInArray;
	Game::Ped* nearestPed = nullptr;
	Game::SCR_f nearestDistanceSquared = std::numeric_limits<Game::SCR_f>::max();

	while (currentPed) {
		if (currentPed->id == 0 || currentPed == skipPed) {
			currentPed = currentPed->nextPed;
			continue;
		}

		Game::SCR_Vector3* pedPosition = GetPedPosition(currentPed);
		if (!pedPosition) {
			currentPed = currentPed->nextPed;
			continue;
		}

		Game::SCR_f dx = pedPosition->x - basePosition.x;
		Game::SCR_f dy = pedPosition->y - basePosition.y;
		Game::SCR_f dz = pedPosition->z - basePosition.z;

		Game::SCR_f distanceSquared =
			Game::Utils::Multiply(dx, dx) +
			Game::Utils::Multiply(dy, dy) +
			Game::Utils::Multiply(dz, dz);

		if (distanceSquared < nearestDistanceSquared) {
			nearestDistanceSquared = distanceSquared;
			nearestPed = currentPed;
		}

		currentPed = currentPed->nextPed;
	}

	return nearestPed;
}
