#include "unlock-all-levels.h"

bool ModMenuModule::Utils::UnlockAllLevels()
{
	Game::S28* s28 = Game::Memory::GetS28();
	if (!s28) {
		spdlog::warn("Could not unlock all levels: S28 is null");
		return false;
	}

	Game::Functions::UnlockAllLevels(s28);
	spdlog::info("All levels unlocked");
	return true;
}
