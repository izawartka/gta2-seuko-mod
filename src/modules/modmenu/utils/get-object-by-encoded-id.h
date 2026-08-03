#pragma once
#include "../common.h"

namespace ModMenuModule::Utils {
	using EncodedObjectId = int;
	// 0 - ... = by id
	// -1 = no object 
	// -2 = nearest object

	Game::BareGameObject* GetObjectByEncodedId(EncodedObjectId objectId, const Game::SCR_Vector3& basePosition, const Game::BareGameObject* nearestObjectIgnore = nullptr);
}
