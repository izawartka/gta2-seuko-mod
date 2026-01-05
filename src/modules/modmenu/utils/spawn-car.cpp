#include "spawn-car.h"

Game::Car* ModMenuModule::Utils::SpawnCar(Game::SCR_Vector3 position, short rotation, Game::CAR_MODEL4 model, short remap, Game::PALETTE_BASE palette)
{
	Game::TrafficManager* trafficManager = Game::Memory::GetTrafficManager();
	if (!trafficManager) {
		spdlog::error("Cannot spawn object: TrafficManager is null.");
		return nullptr;
	}

	int* scale = Game::Memory::GetCarsScale();

	Game::Car* car = Game::Functions::SpawnCar(
		trafficManager,
		0,
		position.x,
		position.y,
		position.z,
		rotation,
		model,
		*scale
	);

	if (!car || !car->sprite) {
		spdlog::error("Cannot spawn vehicle: Failed to spawn car model #{}.", static_cast<uint32_t>(model));
		return nullptr;
	}

	car->sprite->carColor = remap;
	car->sprite->paletteBase = palette;

	spdlog::info("Spawned car model #{}.", static_cast<uint32_t>(model));

	return car;
}
