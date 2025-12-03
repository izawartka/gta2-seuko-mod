#include "camera-pos.h"
#include "../../cheat-registry.h"

ModMenuModule::CameraPosCheat::CameraPosCheat() : ModMenuModule::CheatBase("Cheat_CameraPos_IsEnabled") {
}

ModMenuModule::CameraPosCheat::~CameraPosCheat()
{
}

void ModMenuModule::CameraPosCheat::SetFromCurrentPos()
{
	if(!IsEnabled()) {
		spdlog::warn("CameraPosCheat::SetFromCurrentPos: Cheat is not enabled, cannot set from current");
		return;
	}
	m_setFromCurrentRequested = true;
}

void ModMenuModule::CameraPosCheat::SetFromOriginalTargetPos()
{
	if(!IsEnabled()) {
		spdlog::warn("CameraPosCheat::SetFromOriginalTargetPos: Cheat is not enabled, cannot set from player");
		return;
	}
	m_setFromOriginalTargetRequested = true;
}

void ModMenuModule::CameraPosCheat::SetOptions(const CameraPosOptions& options)
{
	if (!IsEnabled()) {
		spdlog::warn("CameraPosCheat::SetOptions: Cheat is not enabled, cannot set options");
		return;
	}
	m_options = options;
}

void ModMenuModule::CameraPosCheat::OnFirstEnable()
{
	m_camera1Resolver = Core::MakeResolver(
		Game::Memory::GetGame,
		mem(&Game::Game::CurrentPlayer),
		mem(&Game::Player::ph1)
	);

	m_camera2Resolver = Core::MakeResolver(
		Game::Memory::GetGame,
		mem(&Game::Game::CurrentPlayer),
		mem(&Game::Player::ph2)
	);
}

void ModMenuModule::CameraPosCheat::OnEnable()
{
	m_setFromCurrentRequested = false;
	m_setFromOriginalTargetRequested = false;
	m_options = {};
	AddEventListener<GameTickEvent>(&ModMenuModule::CameraPosCheat::OnGameTick);
}

void ModMenuModule::CameraPosCheat::OnDisable()
{
	RemoveEventListener<GameTickEvent>();
	AddEventListener<GameTickEvent>(&ModMenuModule::CameraPosCheat::OnGameTick, true);
	m_setFromCurrentRequested = false;
	m_setFromOriginalTargetRequested = true;
}

void ModMenuModule::CameraPosCheat::OnGameTick(GameTickEvent& event)
{
	Game::Camera* camera1 = m_camera1Resolver();
	Game::Camera* camera2 = m_camera2Resolver();

	if (m_setFromOriginalTargetRequested) {
		if (camera2) {
			m_options.lockedX = camera2->cameraPosTarget.x;
			m_options.lockedY = camera2->cameraPosTarget.y;
			m_options.lockedZ = camera2->cameraPosTarget.z;
		}
	}

	if (m_setFromCurrentRequested) {
		if (camera2) {
			m_options.lockedX = camera2->cameraPos.x;
			m_options.lockedY = camera2->cameraPos.y;
			m_options.lockedZ = camera2->cameraPos.z;
		}

		m_setFromCurrentRequested = false;
	}

	if (!camera1 || !camera2) return;

	if (m_options.lockedX.has_value()) {
		camera1->cameraPos.x = m_options.lockedX.value();
		camera1->cameraPosTarget.x = m_options.lockedX.value();
		camera2->cameraPos.x = m_options.lockedX.value();
		camera2->cameraPosTarget.x = m_options.lockedX.value();
	}

	if (m_options.lockedY.has_value()) {
		camera1->cameraPos.y = m_options.lockedY.value();
		camera1->cameraPosTarget.y = m_options.lockedY.value();
		camera2->cameraPos.y = m_options.lockedY.value();
		camera2->cameraPosTarget.y = m_options.lockedY.value();
	}

	if (m_options.lockedZ.has_value()) {
		camera1->cameraPos.z = m_options.lockedZ.value();
		camera1->cameraPosTarget.z = m_options.lockedZ.value();
		camera2->cameraPos.z = m_options.lockedZ.value();
		camera2->cameraPosTarget.z = m_options.lockedZ.value();
	}
}

REGISTER_CHEAT(CameraPosCheat);
