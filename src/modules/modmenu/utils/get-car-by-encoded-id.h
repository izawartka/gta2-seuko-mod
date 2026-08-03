#pragma once
#include "../common.h"

namespace ModMenuModule::Utils {
	using EncodedCarId = int;
	// 0 - ... = by id
	// -1 = no car
	// -2 = main player ped's car
	// -3 = current player ped's car
	// -4 = current/last car (from LastCarCheat)
	// -5 = nearest car

	Game::Car* GetCarByEncodedId(EncodedCarId carId, const Game::SCR_Vector3& basePosition, const Game::Car* nearestCarIgnore = nullptr);
}
