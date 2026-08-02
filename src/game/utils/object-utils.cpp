#include "object-utils.h"
#include "../memory.h"

Game::BareGameObject* Game::Utils::GetObjectById(int id)
{
	BareGameObjectManager* objectManager = Memory::GetBareGameObjectManager();
	if (!objectManager) return nullptr;

	for (int i = 0; i < 3825; i++) {
		BareGameObject* object = &objectManager->objects[i];
		if (object->id == id) {
			return object;
		}
	}

	return nullptr;
}
