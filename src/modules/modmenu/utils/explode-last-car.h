#pragma once
#include "../common.h"

namespace ModMenuModule::Utils {
	enum class ExplodeLastCarResult {
		Success,
		AlreadyExploded,
		NoLastCar,
	};

	ExplodeLastCarResult ExplodeLastCar(Game::EXPLOSION_SIZE explosionSize = Game::EXPLOSION_SIZE_MEDIUM);
	ExplodeLastCarResult ExplodeLastCarWithToast(Game::EXPLOSION_SIZE explosionSize = Game::EXPLOSION_SIZE_MEDIUM, bool showSuccessToast = true);
}
