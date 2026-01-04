#include "spawn-object.h"
#include "spawn-object-at-player.h"
#include "../cheats/player-pos.h"

Game::GameObject* ModMenuModule::Utils::SpawnObjectAtPlayer(Game::OBJECT_TYPE objectType)
{
	PlayerPosCheat* playerPosCheat = PlayerPosCheat::GetInstance();
	if (!playerPosCheat->IsEnabled()) {
		spdlog::warn("Cannot spawn object: Player position cheat is not enabled.");
		return nullptr;
	}

	const auto& positionOpt = playerPosCheat->GetLastPosition();
	const auto& rotationOpt = playerPosCheat->GetLastRotation();

	if (!positionOpt.has_value() || !rotationOpt.has_value()) {
		spdlog::warn("Cannot spawn object: Player position or rotation is invalid.");
		return nullptr;
	}

	return Utils::SpawnObject(
		positionOpt.value(),
		rotationOpt.value(),
		objectType
	);
}
