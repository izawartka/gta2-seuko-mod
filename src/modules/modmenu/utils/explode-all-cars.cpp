#include "explode-all-cars.h"
#include "get-cars-sorted-by-distance.h"

namespace ModMenuModule::Utils {
	void ExplodeAllCars(bool excludePlayerCar, Game::EXPLOSION_SIZE explosionSize)
	{
		Game::Ped* playerPed = Game::Memory::GetPlayerPed();
		if (!playerPed) {
			spdlog::warn("Cannot explode all cars: player ped is null");
			return;
		}

		std::vector<CarDistanceInfo> cars = GetCarsSortedByDistance();
		if (cars.empty()) return;

		auto explodeCar = [excludePlayerCar, explosionSize](Game::Car* car) -> bool {
			if (car->carDamage >= 32001) return false; // already exploded
			if (excludePlayerCar && car == Game::Memory::GetPlayerPed()->currentCar) return false;
			Game::Functions::ExplodeCar(car, 0, explosionSize);

			return true;
		};

		// explode 1 car far away first because the first explosion is invisible for some reason
		int firstExplodedIndex = -1;
		for (int i = cars.size() - 1; i >= 0; i--) {
			Game::Car* car = cars[i].car;
			if (explodeCar(car)) {
				firstExplodedIndex = i;
				break;
			}
		}

		for (int i = 0; i < firstExplodedIndex; i++) {
			Game::Car* car = cars[i].car;
			explodeCar(car);
		}

		spdlog::info("Exploded all cars");
	}
}
