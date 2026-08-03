#include "get-ped-by-encoded-id.h"
#include "get-nearest-ped.h"

Game::Ped* ModMenuModule::Utils::GetPedByEncodedId(EncodedPedId pedId, const Game::SCR_Vector3& basePosition, const Game::Ped* nearestPedIgnore)
{
	if (pedId >= 0) return Game::Functions::GetPedById(pedId);

	switch (pedId) {
	case -1: return nullptr;
	case -2: {
		Game::Player* player = Game::Utils::GetPlayer();
		if (!player) return nullptr;
		return player->ped;
	}
	case -3: {
		return Game::Utils::GetPlayerCurrentPed();
	}
	case -4: {
		Game::Ped* nearestPed = ModMenuModule::Utils::GetNearestPed(basePosition, nearestPedIgnore);
		if (!nearestPed) {
			spdlog::warn("GetPedByEncodedId: No ped found near position ({}, {}, {})", basePosition.x, basePosition.y, basePosition.z);
			return nullptr;
		}
		return nearestPed;
	}
	default: {
		spdlog::warn("GetPedByEncodedId: Unsupported pedId: {}", pedId);
		return nullptr;
	}
	}
}
