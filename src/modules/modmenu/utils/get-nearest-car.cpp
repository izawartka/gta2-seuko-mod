#include "get-nearest-car.h"

Game::Car* ModMenuModule::Utils::GetNearestCar(Game::SCR_Vector3 basePosition, Game::Car* skipCar)
{
	Game::CarsManager* carsManager = Game::Memory::GetCarsManager();
	if (!carsManager) {
		spdlog::warn("Cannot get nearest car: CarsManager is null");
		return {};
	}

	Game::Car* nearestCar = nullptr;
	Game::SCR_f nearestDistanceSquared = std::numeric_limits<Game::SCR_f>::max();

	for (int i = 0; i < 306; i++) {
		Game::Car* car = &carsManager->arr306Cars[i];
		if (!car->sprite || car->id == 0 || car == skipCar) continue;

		Game::SCR_f dx = car->sprite->position.x - basePosition.x;
		Game::SCR_f dy = car->sprite->position.y - basePosition.y;
		Game::SCR_f dz = car->sprite->position.z - basePosition.z;

		Game::SCR_f distanceSquared =
			Game::Utils::Multiply(dx, dx) +
			Game::Utils::Multiply(dy, dy) +
			Game::Utils::Multiply(dz, dz);

		if (distanceSquared < nearestDistanceSquared) {
			nearestDistanceSquared = distanceSquared;
			nearestCar = car;
		}
	}

	return nearestCar;
}
