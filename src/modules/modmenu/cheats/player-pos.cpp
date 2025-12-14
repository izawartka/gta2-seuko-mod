#include "player-pos.h"
#include "../events/player-pos-update.h"
#include "../cheat-registry.h"

ModMenuModule::PlayerPosCheat::PlayerPosCheat() : ModMenuModule::CheatBase("") {
}

ModMenuModule::PlayerPosCheat::~PlayerPosCheat()
{
}

void ModMenuModule::PlayerPosCheat::Teleport(Game::SCR_f x, Game::SCR_f y)
{
	if (!IsEnabled()) {
		spdlog::warn("PlayerPosCheat::Teleport: Cheat is not enabled, cannot teleport");
	}

	m_teleportPosition = { x, y, 0 };
	m_teleportAutoZ = true;
}

void ModMenuModule::PlayerPosCheat::Teleport(Game::SCR_f x, Game::SCR_f y, Game::SCR_f z)
{
	if (!IsEnabled()) {
		spdlog::warn("PlayerPosCheat::Teleport: Cheat is not enabled, cannot teleport");
	}

	m_teleportPosition = { x, y, z };
	m_teleportAutoZ = false;
}

void ModMenuModule::PlayerPosCheat::OnFirstEnable()
{

}

void ModMenuModule::PlayerPosCheat::OnEnable()
{
	AddEventListener<PreGameTickEvent>(&ModMenuModule::PlayerPosCheat::OnPreGameTick);
	AddEventListener<GameEndEvent>(&ModMenuModule::PlayerPosCheat::OnGameEnd);
}

void ModMenuModule::PlayerPosCheat::OnDisable()
{
	RemoveEventListener<PreGameTickEvent>();
	RemoveEventListener<GameEndEvent>();
	m_position = std::nullopt;
	m_teleportPosition = std::nullopt;
	m_teleportAutoZ = false;
	m_cameraSyncTimer = 0;
}

void ModMenuModule::PlayerPosCheat::OnPreGameTick(PreGameTickEvent& event)
{
	if (m_teleportPosition.has_value()) {
		bool teleported = false;

		if (m_teleportAutoZ) {
			teleported = Game::Utils::TeleportPlayer(
				m_teleportPosition->x,
				m_teleportPosition->y
			);
		}
		else {
			teleported = Game::Utils::TeleportPlayer(
				m_teleportPosition->x,
				m_teleportPosition->y,
				m_teleportPosition->z
			);
		}

		if (teleported) {
			m_cameraSyncTimer = 3;
		}

		m_teleportPosition = std::nullopt;
		m_teleportAutoZ = false;
	}

	UpdateCameraSync();

	Game::Ped* playerPed = Game::Memory::GetPlayerPed();

	std::optional<PlayerPosCheatPos> lastPosition = m_position;

	if(playerPed->gameObject && playerPed->gameObject->sprite) {
		m_position = PlayerPosCheatPos{
			playerPed->gameObject->sprite->x,
			playerPed->gameObject->sprite->y,
			playerPed->gameObject->sprite->z
		};
	}
	else if (playerPed->currentCar && playerPed->currentCar->sprite) {
		m_position = PlayerPosCheatPos{
			playerPed->currentCar->sprite->x,
			playerPed->currentCar->sprite->y,
			playerPed->currentCar->sprite->z
		};
	}
	else {
		m_position = std::nullopt;
	}

	if (lastPosition != m_position) {
		PlayerPosUpdateEvent event(m_position);
		Core::EventManager::GetInstance()->Dispatch(event);
	}
}

void ModMenuModule::PlayerPosCheat::OnGameEnd(GameEndEvent& event)
{
	m_position = std::nullopt;
	m_teleportPosition = std::nullopt;
	m_teleportAutoZ = false;
	m_cameraSyncTimer = 0;
}

void ModMenuModule::PlayerPosCheat::UpdateCameraSync()
{
	if (m_cameraSyncTimer == 0) return;
	m_cameraSyncTimer--;
	if (m_cameraSyncTimer != 0) return;

	Game::Game* game = Game::Memory::GetGame();
	if (!game) return;

	Game::Player* player = game->CurrentPlayer;
	if (!player) return;

	spdlog::debug("PlayerPosCheat: Syncing camera position to target position after teleport");
	player->ph1.cameraPos = player->ph1.cameraPosTarget2;
}

REGISTER_CHEAT(PlayerPosCheat)
