#pragma once
#include "../common.h"

namespace ModMenuModule::Utils {
	Game::GameObject* SpawnObject(Game::SCR_Vector3 position, short rotation, Game::OBJECT_TYPE objectType);
}
