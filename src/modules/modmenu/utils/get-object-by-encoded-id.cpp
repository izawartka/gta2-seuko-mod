#include "get-object-by-encoded-id.h"
#include "get-nearest-object.h"

Game::BareGameObject* ModMenuModule::Utils::GetObjectByEncodedId(EncodedObjectId objectId, const Game::SCR_Vector3& basePosition, const Game::BareGameObject* nearestObjectIgnore)
{
	if (objectId >= 0) return Game::Utils::GetObjectById(objectId);

	switch (objectId) {
	case -1: return nullptr;
	case -2: {
		Game::BareGameObject* nearestObject = ModMenuModule::Utils::GetNearestObject(basePosition, nearestObjectIgnore);
		if (!nearestObject) {
			spdlog::warn("GetObjectByEncodedId: No object found near position ({}, {}, {})", basePosition.x, basePosition.y, basePosition.z);
			return nullptr;
		}
		return nearestObject;
	}
	default: {
		spdlog::warn("GetObjectByEncodedId: Unsupported objectId: {}", objectId);
		return nullptr;
	}
	}
}
