#include "get-car-by-encoded-id.h"
#include "../cheats/last-car.h"
#include "get-nearest-car.h"

Game::Car* ModMenuModule::Utils::GetCarByEncodedId(EncodedCarId carId, const Game::SCR_Vector3& basePosition, const Game::Car* nearestCarIgnore)
{
	if (carId >= 0) return Game::Utils::GetCarById(carId);

	switch (carId) {
	case -1: return nullptr;
	case -2: {
		Game::Player* player = Game::Utils::GetPlayer();
		if (!player || !player->ped) return nullptr;
		return player->ped->currentCar;
	}
	case -3: {
		Game::Ped* currentPed = Game::Utils::GetPlayerCurrentPed();
		if (!currentPed) return nullptr;
		return currentPed->currentCar;
	}
	case -4: {
		ModMenuModule::LastCarCheat* lastCarCheat = ModMenuModule::LastCarCheat::GetInstance();
		if (!lastCarCheat->IsEnabled()) {
			spdlog::warn("GetCarByEncodedId: Last car cheat is not enabled.");
			return nullptr;
		}

		Game::Car* lastCar = lastCarCheat->GetLastCar();
		if (!lastCar) {
			spdlog::warn("GetCarByEncodedId: No last car found.");
			return nullptr;
		}

		return lastCar;
	}
	case -5: {
		Game::Car* nearestCar = ModMenuModule::Utils::GetNearestCar(basePosition);
		if (!nearestCar) {
			spdlog::warn("GetCarByEncodedId: No car found near position ({}, {}, {})", basePosition.x, basePosition.y, basePosition.z);
			return nullptr;
		}
		return nearestCar;
	}
	default: {
		spdlog::warn("GetCarByEncodedId: Unsupported carId: {}", carId);
		return nullptr;
	}
	}
}
