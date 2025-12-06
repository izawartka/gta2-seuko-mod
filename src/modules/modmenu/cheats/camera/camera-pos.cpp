#include "camera-pos.h"
#include "../../cheat-registry.h"

ModMenuModule::CameraPosCheat::CameraPosCheat() : ModMenuModule::CheatBase("Cheat_CameraPos_IsEnabled") {
}

ModMenuModule::CameraPosCheat::~CameraPosCheat()
{
}

void ModMenuModule::CameraPosCheat::LockAtCurrentPos()
{
	if(!IsEnabled()) {
		spdlog::warn("CameraPosCheat::LockAtCurrentPos: Cheat is not enabled, cannot lock at current pos");
		return;
	}
	m_lockAtCurrentRequested = true;
}

void ModMenuModule::CameraPosCheat::SnapToTargetPos()
{
	if (!IsEnabled()) {
		spdlog::warn("CameraPosCheat::SnapToTargetPos: Cheat is not enabled, cannot snap to target pos");
		return;
	}
	m_snapToTargetRequested = true;
}

void ModMenuModule::CameraPosCheat::SnapAndDisable()
{
	if (!IsEnabled()) {
		spdlog::warn("CameraPosCheat::SnapAndDisable: Cheat is not enabled, cannot snap and disable");
		return;
	}
	m_snapToTargetRequested = true;
	m_snapAndDisableRequested = true;
}

void ModMenuModule::CameraPosCheat::SetOptions(const CameraPosCheatOptions& options)
{
	if (!IsEnabled()) {
		spdlog::warn("CameraPosCheat::SetOptions: Cheat is not enabled, cannot set options");
		return;
	}
	m_options = options;
}

void ModMenuModule::CameraPosCheat::OnFirstEnable()
{

}

void ModMenuModule::CameraPosCheat::OnEnable()
{
	AddEventListener<CameraPosApplyEvent>(&ModMenuModule::CameraPosCheat::OnCameraPosApply);
}

void ModMenuModule::CameraPosCheat::OnDisable()
{
	RemoveEventListener<CameraPosApplyEvent>();
	m_lockAtCurrentRequested = false;
	m_snapToTargetRequested = false;
	m_snapAndDisableRequested = false;
}

void ModMenuModule::CameraPosCheat::OnCameraPosApply(CameraPosApplyEvent& event)
{
	event.SetDoApply(false);

	Game::Camera* camera = event.GetCamera();
	if (!camera) return;

	if (m_lockAtCurrentRequested) {
		m_options.x = { CameraPosCheatMode::LockTargetAt, camera->cameraPos.x };
		m_options.y = { CameraPosCheatMode::LockTargetAt, camera->cameraPos.y };
		m_options.z = { CameraPosCheatMode::LockTargetAt, camera->cameraPos.z };
		m_options.zoom = { CameraPosCheatMode::LockTargetAt, camera->cameraPos.zoom };
		m_lockAtCurrentRequested = false;
	}

	camera->cameraPosTarget2 = camera->cameraPosTarget;

	if (m_snapAndDisableRequested) {
		camera->cameraPos = camera->cameraPosTarget2;
		SetEnabled(false);
		return;
	}

	ApplyCoordinate(m_options.x, camera->cameraPos.x, camera->cameraPosTarget2.x);
	ApplyCoordinate(m_options.y, camera->cameraPos.y, camera->cameraPosTarget2.y);
	ApplyCoordinate(m_options.z, camera->cameraPos.z, camera->cameraPosTarget2.z);
	ApplyCoordinate(m_options.zoom, camera->cameraPos.zoom, camera->cameraPosTarget2.zoom);

	if (m_options.reverseZMinLock && m_options.z.mode != CameraPosCheatMode::LockTargetAt) {
		ApplyReverseZMinLock(camera);
	}

	m_snapToTargetRequested = false;
}

void ModMenuModule::CameraPosCheat::ApplyCoordinate(CameraPosCheatCoordinate& coord, Game::SCR_f& camCoord, Game::SCR_f& camCoordTarget2) const
{
	switch (coord.mode) {
	case CameraPosCheatMode::LockTargetAt:
		camCoordTarget2 = coord.value;
		break;
	case CameraPosCheatMode::IncrementTargetBy:
		camCoordTarget2 += coord.value;
		break;
	case CameraPosCheatMode::Unmodified:
	default:
		break;
	}

	if (coord.lockAtTarget || m_snapToTargetRequested) {
		camCoord = camCoordTarget2;
	}
}

void ModMenuModule::CameraPosCheat::ApplyReverseZMinLock(Game::Camera* camera) const
{
	Game::Ped* playerPed = Game::Memory::GetPlayerPed();
	if (!playerPed) return;

	camera->cameraPosTarget2.z += std::min(Game::Utils::FromFloat(3.0f), playerPed->z) - Game::Utils::FromFloat(2.0f);
}

REGISTER_CHEAT(CameraPosCheat);
