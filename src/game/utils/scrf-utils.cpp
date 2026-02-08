#include "scrf-utils.h"

bool Game::Utils::IsCoordSafe(SCR_f coord)
{
	return coord > FromFloat(1.0f) && coord < FromFloat(254.0f);
}

bool Game::Utils::IsZCoordSafe(SCR_f zCoord)
{
	return zCoord >= FromFloat(1.0f) && zCoord < FromFloat(8.0f);
}

bool Game::Utils::IsPositionSafe(SCR_Vector3 position)
{
	return IsCoordSafe(position.x) &&
		IsCoordSafe(position.y) &&
		IsZCoordSafe(position.z);
}

Game::SCR_f Game::Utils::ClampCoordToSafe(SCR_f coord)
{
	return std::clamp(coord, FromFloat(1.0f) + 1, FromFloat(254.0f) - 1);
}

Game::SCR_f Game::Utils::ClampZCoordToSafe(SCR_f zCoord)
{
	return std::clamp(zCoord, FromFloat(1.0f), FromFloat(8.0f) - 1);
}

Game::SCR_Vector3 Game::Utils::ClampPositionToSafe(SCR_Vector3 position)
{
	return {
		ClampCoordToSafe(position.x),
		ClampCoordToSafe(position.y),
		ClampCoordToSafe(position.z)
	};
}
