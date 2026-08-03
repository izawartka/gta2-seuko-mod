#include "get-nearest-object.h"

Game::BareGameObject* ModMenuModule::Utils::GetNearestObject(Game::SCR_Vector3 basePosition, const Game::BareGameObject* skipObject)
{
	Game::BareGameObjectManager* objectManager = Game::Memory::GetBareGameObjectManager();
	if (!objectManager) {
		spdlog::warn("Cannot get nearest object: BareGameObjectManager is null");
		return nullptr;
	}

	Game::BareGameObject* nearestObject = nullptr;
	Game::SCR_f nearestDistanceSquared = std::numeric_limits<Game::SCR_f>::max();

	for (int i = 0; i < 3825; i++) {
		Game::BareGameObject* currentObject = &objectManager->objects[i];
		if (currentObject->id == 0 || !currentObject->sprite2 || currentObject == skipObject) {
			continue;
		}

		Game::SCR_Vector3& objectPosition = currentObject->sprite2->position;
		Game::SCR_f dx = objectPosition.x - basePosition.x;
		Game::SCR_f dy = objectPosition.y - basePosition.y;
		Game::SCR_f dz = objectPosition.z - basePosition.z;

		Game::SCR_f distanceSquared =
			Game::Utils::Multiply(dx, dx) +
			Game::Utils::Multiply(dy, dy) +
			Game::Utils::Multiply(dz, dz);

		if (distanceSquared < nearestDistanceSquared) {
			nearestDistanceSquared = distanceSquared;
			nearestObject = currentObject;
		}
	}

	return nearestObject;
}
