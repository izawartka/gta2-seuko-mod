#include "camera-pos.h"
#include "../../cheat-registry.h"

ModMenuModule::CameraPosCheat::CameraPosCheat() : ModMenuModule::CheatBase("Cheat_CameraPos_IsEnabled") {
}

ModMenuModule::CameraPosCheat::~CameraPosCheat()
{
}

const std::vector<ModMenuModule::CameraPosCheatMode> ModMenuModule::CameraPosCheat::GetAllCameraPosCheatModes()
{
	return {
		CameraPosCheatMode::Unmodified,
		CameraPosCheatMode::LockTargetAt,
		CameraPosCheatMode::IncrementTargetBy
	};
}

void ModMenuModule::CameraPosCheat::ResetPosOptions()
{
	if (!IsEnabled()) {
		spdlog::warn("CameraPosCheat::ResetPosOptions: Cheat is not enabled, cannot reset pos options");
		return;
	}
	m_options.x = { CameraPosCheatMode::Unmodified, Game::Utils::FromFloat(0.0f), false };
	m_options.y = { CameraPosCheatMode::Unmodified, Game::Utils::FromFloat(0.0f), false };
	m_options.z = { CameraPosCheatMode::Unmodified, Game::Utils::FromFloat(0.0f), false };
	m_options.zoom = { CameraPosCheatMode::Unmodified, Game::Utils::FromFloat(0.0f), false };
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
	LoadFromPersistence();
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

	SaveToPersistence();
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

void ModMenuModule::CameraPosCheat::SaveToPersistence() const
{
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();
	size_t dataSize = 1 + sizeof(CameraPosCheatOptions);
	std::unique_ptr<uint8_t[]> dataPtr = std::make_unique<uint8_t[]>(dataSize);

	dataPtr[0] = 1; // version
	memcpy(dataPtr.get() + 1, &m_options, sizeof(CameraPosCheatOptions));

	persistence->SaveRaw("Cheat_CameraPos_State", dataPtr.get(), dataSize);
}

void ModMenuModule::CameraPosCheat::LoadFromPersistence()
{
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();
	std::unique_ptr<uint8_t[]> dataPtr = nullptr;
	size_t dataSize = 0;
	if (!persistence->LoadRaw("Cheat_CameraPos_State", dataPtr, dataSize)) return;
	if (dataSize < 1) {
		spdlog::error("CameraPosCheat::LoadFromPersistence: invalid data size");
		return;
	}
	uint8_t version = dataPtr[0];
	if (version != 1) {
		spdlog::error("CameraPosCheat::LoadFromPersistence: unsupported version {}", version);
		return;
	}
	if (dataSize != 1 + sizeof(CameraPosCheatOptions)) {
		spdlog::error("CameraPosCheat::LoadFromPersistence: invalid data size for version {}", version);
		return;
	}
	memcpy(&m_options, dataPtr.get() + 1, sizeof(CameraPosCheatOptions));
	SetOptions(m_options);
}

REGISTER_CHEAT(CameraPosCheat);
