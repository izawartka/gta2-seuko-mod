#include "spawn-object.h"

Game::GameObject* ModMenuModule::Utils::SpawnObject(Game::SCR_Vector3 position, short rotation, Game::OBJECT_TYPE objectType)
{
	Game::S33* s33 = Game::Memory::GetS33();
	if(!s33) {
		spdlog::error("Cannot spawn object: S33 is null.");
		return nullptr;
	}

	Game::GameObject* spawnedObject = Game::Functions::SpawnObject(
		s33,
		0,
		objectType,
		position.x,
		position.y,
		position.z,
		rotation
	);

	if (spawnedObject) {
		spdlog::info("Spawned object of type #{}", static_cast<int>(objectType));
	}
	else {
		spdlog::error("Cannot spawn object: SpawnObject function returned null.");
	}

	return spawnedObject;
}
