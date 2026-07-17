#include "car-utils.h"
#include "../memory.h"

Game::Car* Game::Utils::GetCarById(uint id)
{
	CarsManager* carsManager = Memory::GetCarsManager();
	if (!carsManager) return nullptr;

	Car* car = carsManager->lastCar;
	while (car)
	{
		if (car->id == id) {
			return car;
		}

		car = car->lastCar;
	}

	return nullptr;
}
