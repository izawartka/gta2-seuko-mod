#pragma once
#include "../common.h"

namespace ModMenuModule::Utils {
	Game::BareGameObject* GetNearestObject(Game::SCR_Vector3 basePosition, const Game::BareGameObject* skipObject = nullptr);
}
