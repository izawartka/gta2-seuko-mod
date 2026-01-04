#include "spawn-car-at-player.h"
#include "spawn-car.h"
#include "../cheats/player-pos.h"

Game::Car* ModMenuModule::Utils::SpawnCarAtPlayer(Game::CAR_MODEL4 model, short remap, Game::PALETTE_BASE palette) {
	Game::Ped* ped = Game::Memory::GetPlayerPed();

	if (ped->currentCar) {
		spdlog::warn("Cannot spawn car: Player is already in a vehicle.");
		return nullptr;
	}

	PlayerPosCheat* playerPosCheat = PlayerPosCheat::GetInstance();
	if (!playerPosCheat->IsEnabled()) {
		spdlog::warn("Cannot spawn car: Player position cheat is not enabled.");
		return nullptr;
	}

	const auto& positionOpt = playerPosCheat->GetLastPosition();
	const auto& rotationOpt = playerPosCheat->GetLastRotation();

	if (!positionOpt.has_value() || !rotationOpt.has_value()) {
		spdlog::warn("Cannot spawn car: Player position or rotation is invalid.");
		return nullptr;
	}

	return Utils::SpawnCar(
		positionOpt.value(),
		rotationOpt.value(),
		model,
		remap,
		palette
	);
}
