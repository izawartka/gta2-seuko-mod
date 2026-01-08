#include "get-cars-sorted-by-distance.h"
#include "../cheats/player-pos.h"

std::vector<ModMenuModule::Utils::CarDistanceInfo> ModMenuModule::Utils::GetCarsSortedByDistance()
{
	Game::CarsManager* carsManager = Game::Memory::GetCarsManager();
	if (!carsManager) {
		spdlog::warn("Cannot get cars sorted by distance: CarsManager is null");
		return {};
	}

	PlayerPosCheat* playerPosCheat = PlayerPosCheat::GetInstance();
	if (!playerPosCheat->IsEnabled()) {
		spdlog::warn("Cannot get cars sorted by distance: PlayerPosCheat is not enabled");
		return {};
	}

	const auto& playerPositionOpt = playerPosCheat->GetLastPosition();
	if (!playerPositionOpt.has_value()) {
		spdlog::warn("Cannot get cars sorted by distance: player position is not available");
		return {};
	}

	const auto& playerPosition = playerPositionOpt.value();
	std::vector<CarDistanceInfo> cars;
	cars.reserve(306);

	for (int i = 0; i < 306; i++) {
		Game::Car* car = &carsManager->arr306Cars[i];
		if (!car->sprite || car->id == 0) continue;

		Game::SCR_f dx = car->sprite->position.x - playerPosition.x;
		Game::SCR_f dy = car->sprite->position.y - playerPosition.y;
		Game::SCR_f dz = car->sprite->position.z - playerPosition.z;

		Game::SCR_f distanceSquared =
			Game::Utils::Multiply(dx, dx) +
			Game::Utils::Multiply(dy, dy) +
			Game::Utils::Multiply(dz, dz);

		cars.push_back({ car, distanceSquared });
	}

	std::sort(cars.begin(), cars.end(),
		[](const CarDistanceInfo& a, const CarDistanceInfo& b) {
			return a.distanceSquared < b.distanceSquared;
		}
	);

	return cars;
}
