#include "give-player-powerup.h"

bool ModMenuModule::Utils::GivePlayerPowerup(Game::POWERUP_TYPE powerupType)
{
	Game::Game* game = Game::Memory::GetGame();
	if (!game) {
		spdlog::warn("Could not give power-up: game is null.");
		return false;
	}

	Game::Player* player = game->CurrentPlayer;
	if (!player) {
		spdlog::warn("Could not give power-up: player is null.");
		return false;
	}

	spdlog::info("Giving powerup #{} to player.", static_cast<int>(powerupType));
	Game::Functions::GivePowerUp(player, 0, powerupType);

	return true;
}
