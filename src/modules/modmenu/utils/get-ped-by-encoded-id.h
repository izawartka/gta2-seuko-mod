#pragma once
#include "../common.h"

namespace ModMenuModule::Utils {
	using EncodedPedId = int;
	// 0 - ... = by id
	// -1 = no ped 
	// -2 = main player ped
	// -3 = current player ped
	// -4 = nearest ped

	Game::Ped* GetPedByEncodedId(EncodedPedId pedId, const Game::SCR_Vector3& basePosition, const Game::Ped* nearestPedIgnore = nullptr);
}
