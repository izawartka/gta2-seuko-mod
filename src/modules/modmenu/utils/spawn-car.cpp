#include "spawn-car.h"

Game::Car* ModMenuModule::Utils::SpawnCar(Game::SCR_Vector3 position, short rotation, Game::CAR_MODEL4 model, short remap, Game::PALETTE_BASE palette)
{
	Game::Car* car = Game::Functions::SpawnCar(
		position.x,
		position.y,
		position.z,
		rotation,
		model
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
