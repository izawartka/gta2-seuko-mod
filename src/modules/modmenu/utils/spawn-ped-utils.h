#pragma once
#include "../common.h"
#include "../cheats/position-store.h"
#include "get-ped-by-encoded-id.h"
#include "get-car-by-encoded-id.h"
#include "get-object-by-encoded-id.h"

namespace ModMenuModule::Utils::SpawnPed {
	static constexpr size_t SPAWN_PED_DATA_VERSION = 1;

	struct ObjectiveTargetsData {
		EncodedPedId carId = -1;
		EncodedCarId pedId = -1;
		EncodedObjectId objectId = -1;
		PositionStoreEntry position = {};
	};

	struct SpawnPedData {
		Game::PED_GRAPHIC_TYPE graphicType;
		Game::PED_REMAP remap;
		Game::WEAPON_INDEX weaponIndex; // -1 = no weapon
		Game::ushort health;
		Game::PED_OCCUPATION occupation;
		Game::PED_THREAT_SEARCH threatSearch;
		Game::PED_THREAT_REACTION threatReaction;
		Game::PED_OBJECTIVE objective;
		Game::ushort objectiveTimer;
		ObjectiveTargetsData objectiveTargets;
		Game::PED_STATE state;
		Game::PED_STATE2 state2;
		Game::PED_BIT_STATE bitState;
		Game::PED_BIT_STATE2 bitState2;
		EncodedPedId leaderPedId;
	};

	Game::Ped* SpawnPed(const Game::SCR_Vector3& position, short rotation, const SpawnPedData& data);
	Game::Ped* SpawnPedInNearestCar(const Game::SCR_Vector3& basePosition, const SpawnPedData& data);
	Game::Ped* SpawnPedInCar(const Game::SCR_Vector3& basePosition, EncodedCarId carId, const SpawnPedData& data);
	Game::Ped* SpawnPedAtPlayer(const SpawnPedData& data, bool inNearestCar = false);
}
