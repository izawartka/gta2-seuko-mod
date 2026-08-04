#include "spawn-ped-utils.h"
#include "../cheats/player-pos.h"
#include "../cheats/position-store.h"
#include "get-nearest-car.h"

static void SetPedObjectiveTargets(Game::Ped* ped, const ModMenuModule::Utils::SpawnPed::ObjectiveTargetsData& targetsData, const Game::SCR_Vector3& basePosition)
{
	if (targetsData.carId != -1) {
		Game::Car* targetCar = ModMenuModule::Utils::GetCarByEncodedId(targetsData.carId, basePosition);
		if (targetCar) {
			ped->objectiveTargetCar = targetCar;
		}
		else {
			spdlog::warn("SetPedObjectiveTargets: Car {} not found", targetsData.carId);
		}
	}

	if (targetsData.pedId != -1) {
		Game::Ped* targetPed = ModMenuModule::Utils::GetPedByEncodedId(targetsData.pedId, basePosition, ped);

		if (ped) {
			ped->objectiveTargetPed = targetPed;
		}
		else {
			spdlog::warn("SetPedObjectiveTargets: Ped {} not found", targetsData.pedId);
		}
	}

	if (targetsData.objectId != -1) {
		Game::BareGameObject* targetObject = ModMenuModule::Utils::GetObjectByEncodedId(targetsData.objectId, basePosition);

		if (targetObject) {
			ped->objectiveTargetObject = targetObject;
		}
		else {
			spdlog::warn("SetPedObjectiveTargets: Object {} not found", targetsData.objectId);
		}
	}

	ModMenuModule::PositionStoreEntry targetPosition = targetsData.position;
	ModMenuModule::PositionStoreCheat::ApplyNow(targetPosition);
	ped->objectiveTargetPosition = targetPosition.value.position;
}

static void SetPedLeader(Game::Ped* ped, ModMenuModule::Utils::EncodedPedId leaderId, const Game::SCR_Vector3& basePosition)
{
	if (leaderId == -1) return;

	Game::Ped* leaderPed = ModMenuModule::Utils::GetPedByEncodedId(leaderId, basePosition, ped);
	if (!leaderPed) {
		spdlog::warn("SetPedLeader: Leader ped {} not found", leaderId);
		return;
	}

	if (leaderPed->group) {
		Game::Functions::PedGroupAddPed(leaderPed->group, 0, ped);
	}
	else {
		Game::Functions::PedGroupCreate(ped, 0, 0);
		Game::Functions::PedGroupSetLeader(leaderPed, 0, ped);
	}
}

static Game::Ped* UpdatePedAfterSpawn(Game::Ped* ped, const ModMenuModule::Utils::SpawnPed::SpawnPedData& data, const Game::SCR_Vector3& basePosition)
{
	ped->graphicType = data.graphicType;
	ped->remap = data.remap;

	if (data.weaponIndex != static_cast<Game::WEAPON_INDEX>(-1)) {
		Game::Functions::SetPedWeapon(ped, 0, data.weaponIndex);
	}

	ped->health = data.health;
	ped->occupation = data.occupation;
	ped->threatSearch = data.threatSearch;
	ped->threatReaction = data.threatReaction;

	Game::Functions::PedSetObjective(ped, 0, data.objective, data.objectiveTimer);
	SetPedObjectiveTargets(ped, data.objectiveTargets, basePosition);

	ped->state = data.state;
	ped->state2 = data.state2;
	ped->bitState = data.bitState;
	ped->bitState2 = data.bitState2;

	SetPedLeader(ped, data.leaderPedId, basePosition);

	return ped;
}

Game::Ped* ModMenuModule::Utils::SpawnPed::SpawnPed(const Game::SCR_Vector3& position, short rotation, const SpawnPedData& data)
{
	Game::Ped* ped = Game::Functions::SpawnPed(
		position.x,
		position.y,
		position.z,
		data.remap,
		0
	);

	if (!ped) {
		spdlog::warn("SpawnPed: Failed to spawn ped");
		return nullptr;
	}

	return UpdatePedAfterSpawn(ped, data, position);
}

Game::Ped* ModMenuModule::Utils::SpawnPed::SpawnPedInNearestCar(const Game::SCR_Vector3& basePosition, const ModMenuModule::Utils::SpawnPed::SpawnPedData& data)
{
	Game::Car* car = GetNearestCar(basePosition);

	if (!car) {
		spdlog::warn("SpawnPedInNearestCar: No car found nearby");
		return nullptr;
	}

	if (car->driver) {
		spdlog::warn("SpawnPedInNearestCar: Nearest car already has a driver");
		return nullptr;
	}

	Game::Ped* ped = Game::Utils::CreatePedInCar(car);
	if (!ped) {
		spdlog::warn("SpawnPedInNearestCar: Failed to create ped");
		return nullptr;
	}

	return UpdatePedAfterSpawn(ped, data, basePosition);
}

Game::Ped* ModMenuModule::Utils::SpawnPed::SpawnPedInCar(const Game::SCR_Vector3& basePosition, EncodedCarId carId, const SpawnPedData& data)
{
	Game::Car* car = GetCarByEncodedId(carId, basePosition);

	if (!car) {
		spdlog::warn("SpawnPedInCar: No car found for id {}", carId);
		return nullptr;
	}

	if (car->driver) {
		spdlog::warn("SpawnPedInCar: Specified car already has a driver");
		return nullptr;
	}

	Game::Ped* ped = Game::Utils::CreatePedInCar(car);
	if (!ped) {
		spdlog::warn("SpawnPedInCar: Failed to create ped");
		return nullptr;
	}

	return UpdatePedAfterSpawn(ped, data, basePosition);
}

Game::Ped* ModMenuModule::Utils::SpawnPed::SpawnPedAtPlayer(const SpawnPedData& data)
{
	PlayerPosCheat* playerPosCheat = PlayerPosCheat::GetInstance();
	if (!playerPosCheat->IsEnabled()) {
		spdlog::warn("SpawnPedAtPlayer: Cannot spawn ped, player position cheat is not enabled.");
		return nullptr;
	}

	const auto& positionOpt = playerPosCheat->GetLastPosition();
	const auto& rotationOpt = playerPosCheat->GetLastRotation();

	if (!positionOpt.has_value() || !rotationOpt.has_value()) {
		spdlog::warn("SpawnPedAtPlayer: Cannot spawn ped, player position or rotation is invalid.");
		return nullptr;
	}

	return SpawnPed(
		positionOpt.value(),
		rotationOpt.value(),
		data
	);
}
