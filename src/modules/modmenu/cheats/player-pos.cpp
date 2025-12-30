#include "player-pos.h"
#include "../events/player-pos-update.h"
#include "../events/player-rot-update.h"
#include "../cheat-registry.h"

ModMenuModule::PlayerPosCheat* ModMenuModule::PlayerPosCheat::m_instance = nullptr;

ModMenuModule::PlayerPosCheat::PlayerPosCheat() : ModMenuModule::CheatBase("") {
	assert(m_instance == nullptr && "PlayerPosCheat instance already exists");
	m_instance = this;
}

ModMenuModule::PlayerPosCheat::~PlayerPosCheat()
{
	m_instance = nullptr;
}

ModMenuModule::PlayerPosCheat* ModMenuModule::PlayerPosCheat::GetInstance()
{
	assert(m_instance && "PlayerPosCheat not initialized!");
	return m_instance;
}

void ModMenuModule::PlayerPosCheat::Teleport(Game::SCR_f x, Game::SCR_f y, PlayerPosCheatTeleportCallback callback)
{
	Teleport(Game::SCR_Vector2{x, y}, callback);
}

void ModMenuModule::PlayerPosCheat::Teleport(Game::SCR_f x, Game::SCR_f y, Game::SCR_f z, PlayerPosCheatTeleportCallback callback)
{
	Teleport(Game::SCR_Vector3{ x, y, z }, callback);
}

void ModMenuModule::PlayerPosCheat::Teleport(const Game::SCR_Vector2& position, PlayerPosCheatTeleportCallback callback)
{
	if (!IsEnabled()) {
		spdlog::warn("PlayerPosCheat::Teleport: Cheat is not enabled, cannot teleport");
	}
	
	if (m_teleportCallback) {
		spdlog::warn("PlayerPosCheat::Teleport: Previous teleport is still pending, overwriting callback");
		m_teleportCallback(false);
	}

	m_teleportCallback = callback;
	m_teleportPosition = { position.x, position.y, 0 };
	m_teleportAutoZ = true;
}

void ModMenuModule::PlayerPosCheat::Teleport(const Game::SCR_Vector3& position, PlayerPosCheatTeleportCallback callback)
{
	if (!IsEnabled()) {
		spdlog::warn("PlayerPosCheat::Teleport: Cheat is not enabled, cannot teleport");
	}

	if (m_teleportCallback) {
		spdlog::warn("PlayerPosCheat::Teleport: Previous teleport is still pending, overwriting callback");
		m_teleportCallback(false);
	}
	m_teleportCallback = callback;

	m_teleportPosition = position;
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

	if(m_teleportCallback) {
		m_teleportCallback(false);
		m_teleportCallback = nullptr;
	}
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

		if (m_teleportCallback) {
			m_teleportCallback(teleported);
			m_teleportCallback = nullptr;
		}
	}

	UpdateCameraSync();

	Game::Ped* playerPed = Game::Memory::GetPlayerPed();

	std::optional<Game::SCR_Vector3> lastPosition = m_position;
	std::optional<short> lastRotation = m_rotation;

	if(playerPed->gameObject && playerPed->gameObject->sprite) {
		m_position = playerPed->gameObject->sprite->position;
		m_rotation = playerPed->gameObject->sprite->rotation;
	}
	else if (playerPed->currentCar && playerPed->currentCar->sprite) {
		m_position = playerPed->currentCar->sprite->position;
		m_rotation = playerPed->currentCar->sprite->rotation;
	}
	else {
		m_position = std::nullopt;
		m_rotation = std::nullopt;
	}

	if (lastPosition != m_position) {
		PlayerPosUpdateEvent event(m_position);
		Core::EventManager::GetInstance()->Dispatch(event);
	}

	if (lastRotation != m_rotation) {
		PlayerRotUpdateEvent event(m_rotation);
		Core::EventManager::GetInstance()->Dispatch(event);
	}
}

void ModMenuModule::PlayerPosCheat::OnGameEnd(GameEndEvent& event)
{
	m_position = std::nullopt;
	m_teleportPosition = std::nullopt;
	m_teleportAutoZ = false;
	m_cameraSyncTimer = 0;

	if (m_teleportCallback) {
		m_teleportCallback(false);
		m_teleportCallback = nullptr;
	}
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
