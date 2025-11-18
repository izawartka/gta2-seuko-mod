#pragma once
#include "../common.h"

namespace ModMenuModule::Utils {
	static void ExplodeAllCars(bool excludePlayerCar, Game::EXPLOSION_SIZE explosionSize = Game::EXPLOSION_SIZE_MEDIUM)
	{
		Game::Ped* playerPed = Game::Memory::GetPlayerPed();
		if(!playerPed) {
			spdlog::warn("Cannot explode all cars: player ped is null");
			return;
		}

		Game::CarsManager* carsManager = Game::Memory::GetCarsManager();
		if (!carsManager) {
			spdlog::warn("Cannot explode all cars: cars manager is null");
			return;
		}

		for (int i = 0; i < 306; i++) {
			Game::Car* car = &carsManager->arr306Cars[i];

			if (excludePlayerCar && playerPed->currentCar == car) continue;
			if (!car->sprite || car->id == 0) continue;

			Game::Functions::ExplodeCar(car, 0, explosionSize);
		}
	}
}
