#include "gang-utils.h"
#include "../memory.h"
#include "../functions.h"

std::vector<Game::GangRespect*> Game::Utils::GetVisibleGangs()
{
	std::vector<GangRespect*> gangs;

	GangRespectContainer* container = Memory::GetGangRespectContainer();
	if (!container) {
		return gangs;
	}

	GangRespect* gang = Functions::GetFirstGangRespect(container);
	if (!gang) {
		return gangs;
	}

	do {
		gangs.push_back(gang);
		gang = Functions::GetNextGangRespect(container);
	} while (gang);

	return gangs;
}
