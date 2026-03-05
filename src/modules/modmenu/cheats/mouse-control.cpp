#include "mouse-control.h"
#include "../utils/angle-utils.h"
#include "camera/camera.h"
#include "../toast-manager.h"
#include "../cheat-registry.h"

static constexpr float ROTATE_MODE_SMOOTING = 0.2f;
static constexpr float ROTATION_INPUT_THRESHOLD = 0.11f;
static constexpr float ROTATION_SPEED = 0.20943928f;

ModMenuModule::MouseControlCheat* ModMenuModule::MouseControlCheat::m_instance = nullptr;

ModMenuModule::MouseControlCheat::MouseControlCheat() : ModMenuModule::CheatBase("Cheat_MouseControl_IsEnabled") {
	assert(m_instance == nullptr && "MouseControlCheat instance already exists");
	m_instance = this;
}

ModMenuModule::MouseControlCheat::~MouseControlCheat()
{
	m_instance = nullptr;
}

ModMenuModule::MouseControlCheat* ModMenuModule::MouseControlCheat::GetInstance()
{
	assert(m_instance && "MouseControlCheat not initialized!");
	return m_instance;
}

const std::vector<ModMenuModule::MouseControlCheatMode>& ModMenuModule::MouseControlCheat::GetAllMouseControlCheatModes()
{
	static const std::vector<MouseControlCheatMode> modes = {
		MouseControlCheatMode::Rotate,
		MouseControlCheatMode::PointAt,
	};

	return modes;
}

void ModMenuModule::MouseControlCheat::SetOptions(const MouseControlCheatOptions& options)
{
	if (!IsEnabled()) {
		spdlog::warn("CameraCheat::SetOptions: Cheat is not enabled, cannot set options");
		return;
	}

	SetOptionsInternal(options);
}

void ModMenuModule::MouseControlCheat::OnFirstEnable()
{
	LoadFromPersistence();
}

void ModMenuModule::MouseControlCheat::OnEnable()
{
	AddEventListener<GameStartEvent>(&MouseControlCheat::OnGameStart);
	AddEventListener<GameEndEvent>(&MouseControlCheat::OnGameEnd);
	AddEventListener<GamePauseEvent>(&MouseControlCheat::OnGamePause);
	AddEventListener<GameUnpauseEvent>(&MouseControlCheat::OnGameUnpause);
	UpdateAutoModeListeners();
	UpdateAutoMode();
	CreateControlHandles();
	if (ForceControlsCheat::CheckGameReadyToForce()) Start();
}

void ModMenuModule::MouseControlCheat::OnDisable()
{
	RemoveEventListener<GameStartEvent>();
	RemoveEventListener<GameEndEvent>();
	RemoveEventListener<GamePauseEvent>();
	RemoveEventListener<GameUnpauseEvent>();
	RemoveAutoModeListeners();
	Stop();
	FreeControlHandles();
	SaveToPersistence();
}

void ModMenuModule::MouseControlCheat::OnPreGameTick(PreGameTickEvent& event)
{
	if (CheckShouldUseRotation()) {
		StartRotation();
		UpdateTargetDeltaRotation();
	}
	else {
		StopRotation();
		return;
	}

	char rotationDirection = GetRotationDirection(m_targetDeltaRotation);
	bool shouldRotateLeft = rotationDirection == -1;
	bool shouldRotateRight = rotationDirection == 1;

	if (shouldRotateLeft) m_targetDeltaRotation -= ROTATION_SPEED;
	else if (shouldRotateRight) m_targetDeltaRotation += ROTATION_SPEED;

	ForceControlsCheat* forceControlsCheat = ForceControlsCheat::GetInstance();
	forceControlsCheat->SetControlState(m_leftControlHandle, shouldRotateLeft ? ForceControlState::ForceDown : ForceControlState::ForceUp);
	forceControlsCheat->SetControlState(m_rightControlHandle, shouldRotateRight ? ForceControlState::ForceDown : ForceControlState::ForceUp);
}

void ModMenuModule::MouseControlCheat::OnMouseLockedMove(MouseModule::MouseLockedMoveEvent& event)
{
	int deltaX = event.GetDelta().x;
	m_targetDeltaRotation -= deltaX * m_options.rotateModeSensitivity;
}

void ModMenuModule::MouseControlCheat::OnMouseMove(MouseModule::MouseMoveEvent& event)
{
	MouseModule::MousePosition pos = event.GetPosition();
	m_lastNormalizedPos = MouseModule::MouseManager::ToNormalizedPosition(pos);
}

void ModMenuModule::MouseControlCheat::OnMouseButtonDown(MouseModule::MouseButtonDownEvent& event)
{
	if (event.GetButton() != MouseModule::MouseButton::Left) return;

	ForceControlsCheat* forceControlsCheat = ForceControlsCheat::GetInstance();
	forceControlsCheat->SetControlState(m_attackControlHandle, ForceControlState::ForceDown);
}

void ModMenuModule::MouseControlCheat::OnMouseButtonUp(MouseModule::MouseButtonUpEvent& event)
{
	if (event.GetButton() != MouseModule::MouseButton::Left) return;

	ForceControlsCheat* forceControlsCheat = ForceControlsCheat::GetInstance();
	forceControlsCheat->SetControlState(m_attackControlHandle, ForceControlState::Unmodified);
}

void ModMenuModule::MouseControlCheat::OnGameStart(GameStartEvent& event)
{
	if (ForceControlsCheat::CheckGameReadyToForce()) Start();
}

void ModMenuModule::MouseControlCheat::OnGameEnd(GameEndEvent& event)
{
	Stop();
}

void ModMenuModule::MouseControlCheat::OnGamePause(GamePauseEvent& event)
{
	Stop();
}

void ModMenuModule::MouseControlCheat::OnGameUnpause(GameUnpauseEvent& event)
{
	if (ForceControlsCheat::CheckGameReadyToForce(true)) Start();
}

void ModMenuModule::MouseControlCheat::OnCheatStateChange(CheatStateEvent& event)
{
	if (event.GetCheatType() != typeid(CameraCheat)) return;

	UpdateAutoMode();
}

void ModMenuModule::MouseControlCheat::OnCameraCheatOptionsUpdate(CheatOptionsUpdateEvent<CameraCheat>& event)
{
	if (event.GetOldOptions().followPedRotation == event.GetNewOptions().followPedRotation) return;

	UpdateAutoMode();
}

void ModMenuModule::MouseControlCheat::SetOptionsInternal(const MouseControlCheatOptions& options)
{
	MouseControlCheatOptions oldOptions = m_options;
	m_options = options;

	bool autoModeChanged = (oldOptions.autoMode != m_options.autoMode);
	if (autoModeChanged) {
		if (m_options.autoMode) m_options.mode = GetAutoModeTargetMode();
		if (IsEnabled()) UpdateAutoModeListeners();
	}

	bool modeChanged = (oldOptions.mode != m_options.mode);
	if (m_started && modeChanged) UpdateMode();

	Core::EventManager* eventManager = Core::EventManager::GetInstance();
	CheatOptionsUpdateEvent<MouseControlCheat> event(oldOptions, m_options);
	eventManager->Dispatch(event);
}

void ModMenuModule::MouseControlCheat::UpdateTargetDeltaRotation()
{
	float currentRotation = GetPlayerRotation().value();

	bool isPointAtMode = m_options.mode == MouseControlCheatMode::PointAt;

	if (isPointAtMode) {
		std::optional<float> targetRotationOpt = GetTargetRotation(m_lastNormalizedPos);
		if (!targetRotationOpt.has_value()) {
			m_targetDeltaRotation = 0.0f;
			return;
		}

		float targetDelta = Utils::Angle::GetShortestAngleDifference(currentRotation, targetRotationOpt.value());
		m_targetDeltaRotation = (targetDelta + m_targetDeltaRotation) / 2.0f;
	}
	else {
		m_targetDeltaRotation *= (1.0f - ROTATE_MODE_SMOOTING);
	}
}

void ModMenuModule::MouseControlCheat::UpdateMode()
{
	bool isRotateMode = m_options.mode == MouseControlCheatMode::Rotate;
	bool isPointAtMode = m_options.mode == MouseControlCheatMode::PointAt;

	SetEventListener<MouseModule::MouseLockedMoveEvent>(&MouseControlCheat::OnMouseLockedMove, isRotateMode);
	SetEventListener<MouseModule::MouseMoveEvent>(&MouseControlCheat::OnMouseMove, isPointAtMode);

	MouseModule::MouseManager* mouseManager = MouseModule::MouseManager::GetInstance();
	mouseManager->SetLocked(isRotateMode);
	mouseManager->SetInvisible(isRotateMode);

	if (isPointAtMode) UpdateLastNormalizedPos();
}

void ModMenuModule::MouseControlCheat::UpdateLastNormalizedPos()
{
	MouseModule::MousePosition currentPos = MouseModule::MouseManager::FetchMouseState().position;
	m_lastNormalizedPos = MouseModule::MouseManager::ToNormalizedPosition(currentPos);
}

void ModMenuModule::MouseControlCheat::UpdateAttack() const
{
	MouseModule::MouseState mouseState = MouseModule::MouseManager::FetchMouseState();
	bool shouldAttack = mouseState.leftButtonDown;

	ForceControlsCheat* forceControlsCheat = ForceControlsCheat::GetInstance();
	forceControlsCheat->SetControlState(m_attackControlHandle, shouldAttack ? ForceControlState::ForceDown : ForceControlState::Unmodified);
}

void ModMenuModule::MouseControlCheat::UpdateAutoMode()
{
	if (!m_options.autoMode) return;

	MouseControlCheatOptions newOptions = m_options;
	newOptions.mode = GetAutoModeTargetMode();

	if (newOptions.mode != m_options.mode) {
		SetOptionsInternal(newOptions);
	}
}

void ModMenuModule::MouseControlCheat::UpdateAutoModeListeners()
{
	SetEventListener<CheatStateEvent>(&MouseControlCheat::OnCheatStateChange, m_options.autoMode);
	SetEventListener<CheatOptionsUpdateEvent<CameraCheat>>(&MouseControlCheat::OnCameraCheatOptionsUpdate, m_options.autoMode);
}

void ModMenuModule::MouseControlCheat::RemoveAutoModeListeners()
{
	RemoveEventListener<CheatStateEvent>(true);
	RemoveEventListener<CheatOptionsUpdateEvent<CameraCheat>>(true);
}

bool ModMenuModule::MouseControlCheat::EnsureNotControllerControls() const
{
	ForceControlsCheat::ControlIndex leftControlIndex = ForceControlsCheat::GetControlIndex(Game::KEYBOARD_STATE_LEFT);
	ForceControlsCheat::ControlIndex rightControlIndex = ForceControlsCheat::GetControlIndex(Game::KEYBOARD_STATE_RIGHT);
	ForceControlsCheat::ControlIndex attackControlIndex = ForceControlsCheat::GetControlIndex(Game::KEYBOARD_STATE_ATTACK);

	bool allOk = !ForceControlsCheat::CheckUsesController(leftControlIndex)
		&& !ForceControlsCheat::CheckUsesController(rightControlIndex)
		&& !ForceControlsCheat::CheckUsesController(attackControlIndex);

	if (!allOk) {
		spdlog::warn("Mouse control won't work with Controller controls. Please update your controls in GTA2 Manager");
		ToastManager::GetInstance()->Show({ L"Mouse control won't work with Controller controls.", ToastType::Warning, 240 });
		ToastManager::GetInstance()->Show({ L"Please update your controls in GTA2 Mangager", ToastType::Warning, 240 });
	}

	return allOk;
}

bool ModMenuModule::MouseControlCheat::EnsureControlHandlesOk() const
{
	if (m_controlHandlesOk) return true;

	spdlog::warn("Mouse control cheat cannot start because control handles are not ok");
	ToastManager::GetInstance()->Show({ L"Mouse control cheat has failed to start", ToastType::Error, 240 });
	return false;
}

void ModMenuModule::MouseControlCheat::StartRotation()
{
	if (m_usingRotation) return;
	spdlog::debug("MouseControlCheat: Starting rotation");
	m_usingRotation = true;
	m_targetDeltaRotation = 0.0f;
}

void ModMenuModule::MouseControlCheat::StopRotation()
{
	if (!m_usingRotation) return;
	spdlog::debug("MouseControlCheat: Stopping rotation");
	m_usingRotation = false;
	ForceControlsCheat* forceControlsCheat = ForceControlsCheat::GetInstance();
	forceControlsCheat->SetControlState(m_leftControlHandle, ForceControlState::Unmodified);
	forceControlsCheat->SetControlState(m_rightControlHandle, ForceControlState::Unmodified);
}

void ModMenuModule::MouseControlCheat::Start()
{
	if (m_started) return;
	if (!EnsureControlHandlesOk()) return;
	if (!EnsureNotControllerControls()) return;
	spdlog::debug("MouseControlCheat: Starting");
	m_started = true;

	AddEventListener<PreGameTickEvent>(&MouseControlCheat::OnPreGameTick);
	AddEventListener<MouseModule::MouseButtonDownEvent>(&MouseControlCheat::OnMouseButtonDown);
	AddEventListener<MouseModule::MouseButtonUpEvent>(&MouseControlCheat::OnMouseButtonUp);

	UpdateMode();
	UpdateAttack();
}

void ModMenuModule::MouseControlCheat::Stop()
{
	if (!m_started) return;
	spdlog::debug("MouseControlCheat: Stopping");
	m_started = false;

	RemoveEventListener<PreGameTickEvent>();
	RemoveEventListener<MouseModule::MouseButtonDownEvent>();
	RemoveEventListener<MouseModule::MouseButtonUpEvent>();
	RemoveEventListener<MouseModule::MouseLockedMoveEvent>(true);
	RemoveEventListener<MouseModule::MouseMoveEvent>(true);

	MouseModule::MouseManager* mouseManager = MouseModule::MouseManager::GetInstance();
	mouseManager->SetLocked(false);
	mouseManager->SetInvisible(false);

	ForceControlsCheat* forceControlsCheat = ForceControlsCheat::GetInstance();
	forceControlsCheat->SetControlState(m_attackControlHandle, ForceControlState::Unmodified);
	StopRotation();
}

bool ModMenuModule::MouseControlCheat::CreateControlHandles()
{
	ForceControlsCheat* forceControlsCheat = ForceControlsCheat::GetInstance();

	ForceControlsCheat::ControlIndex leftControlIndex = ForceControlsCheat::GetControlIndex(Game::KEYBOARD_STATE_LEFT);
	m_leftControlHandle = forceControlsCheat->CreateControlHandle(leftControlIndex);
	ForceControlsCheat::ControlIndex rightControlIndex = ForceControlsCheat::GetControlIndex(Game::KEYBOARD_STATE_RIGHT);
	m_rightControlHandle = forceControlsCheat->CreateControlHandle(rightControlIndex);
	ForceControlsCheat::ControlIndex attackControlIndex = ForceControlsCheat::GetControlIndex(Game::KEYBOARD_STATE_ATTACK);
	m_attackControlHandle = forceControlsCheat->CreateControlHandle(attackControlIndex);

	m_controlHandlesOk = (m_leftControlHandle != -1) && (m_rightControlHandle != -1) && (m_attackControlHandle != -1);
	if(!m_controlHandlesOk) {
		spdlog::error("Mouse control cheat failed to create control handles");
		FreeControlHandles();
		return false;
	}

	return true;
}

void ModMenuModule::MouseControlCheat::FreeControlHandles()
{
	ForceControlsCheat* forceControlsCheat = ForceControlsCheat::GetInstance();

	if (m_leftControlHandle != -1) {
		forceControlsCheat->FreeControlHandle(m_leftControlHandle);
		m_leftControlHandle = -1;
	}

	if (m_rightControlHandle != -1) {
		forceControlsCheat->FreeControlHandle(m_rightControlHandle);
		m_rightControlHandle = -1;
	}

	if (m_attackControlHandle != -1) {
		forceControlsCheat->FreeControlHandle(m_attackControlHandle);
		m_attackControlHandle = -1;
	}

	m_controlHandlesOk = false;
}

void ModMenuModule::MouseControlCheat::SaveToPersistence() const
{
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();
	size_t dataSize = 1 + sizeof(MouseControlCheatOptions);
	std::unique_ptr<uint8_t[]> dataPtr = std::make_unique<uint8_t[]>(dataSize);

	dataPtr[0] = 1; // version
	memcpy(dataPtr.get() + 1, &m_options, sizeof(MouseControlCheatOptions));

	persistence->SaveRaw("Cheat_MouseControl_State", dataPtr.get(), dataSize);
}

void ModMenuModule::MouseControlCheat::LoadFromPersistence()
{
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();
	std::unique_ptr<uint8_t[]> dataPtr = nullptr;
	size_t dataSize = 0;
	if (!persistence->LoadRaw("Cheat_MouseControl_State", dataPtr, dataSize)) return;
	if (dataSize < 1) {
		spdlog::error("MouseControlCheat::LoadFromPersistence: invalid data size");
		return;
	}
	uint8_t version = dataPtr[0];
	if (version != 1) {
		spdlog::error("MouseControlCheat::LoadFromPersistence: unsupported version {}", version);
		return;
	}
	if (dataSize != 1 + sizeof(MouseControlCheatOptions)) {
		spdlog::error("MouseControlCheat::LoadFromPersistence: invalid data size for version {}", version);
		return;
	}
	memcpy(&m_options, dataPtr.get() + 1, sizeof(MouseControlCheatOptions));
	SetOptions(m_options);
}

bool ModMenuModule::MouseControlCheat::CheckShouldUseRotation()
{
	Game::Game* game = Game::Memory::GetGame();
	if (!game) return false;
	Game::Player* player = game->currentPlayer;
	if (!player || !player->ped || !player->ped->gameObject) return false;
	if (player->ped->targetCarForEnter) return false;
	return true;
}

char ModMenuModule::MouseControlCheat::GetRotationDirection(float deltaAngle)
{
	if (std::abs(deltaAngle) < ROTATION_INPUT_THRESHOLD) {
		return Game::KEYBOARD_STATE(0);
	}
	return deltaAngle > 0 ? -1 : 1;
}

std::optional<float> ModMenuModule::MouseControlCheat::GetPlayerRotation()
{
	Game::Game* game = Game::Memory::GetGame();
	if (!game) return std::nullopt;
	Game::Player* player = game->currentPlayer;
	if (!player || !player->ped || !player->ped->gameObject) return std::nullopt;
	return Game::Utils::FromGTAAngleToRad(player->ped->gameObject->spriteRotation);
}

std::optional<float> ModMenuModule::MouseControlCheat::GetTargetRotation(MouseModule::NormalizedMousePosition normalizedPos)
{
	normalizedPos.x -= 0.5f;
	normalizedPos.y -= 0.5f;
	normalizedPos.x *= -1.0f;

	CameraCheat* cameraCheat = CameraCheat::GetInstance();
	if (cameraCheat->IsEnabled()) {
		CameraCheatOptions cameraOptions = cameraCheat->GetOptions();
		normalizedPos.x *= std::cos(cameraOptions.cameraTransform.horizontalAngleRad);
		if (normalizedPos.x == 0.0f && normalizedPos.y == 0.0f) return std::nullopt;
		float targetAngle = std::atan2(normalizedPos.y, normalizedPos.x);

		float cameraRotation = cameraCheat->GetOptions().cameraTransform.verticalAngleRad;
		return Utils::Angle::NormalizeAngle(targetAngle + cameraRotation - static_cast<float>(M_PI / 2.0f));
	}
	else {
		return Utils::Angle::NormalizeAngle(std::atan2(normalizedPos.y, normalizedPos.x) - static_cast<float>(M_PI / 2.0f));
	}
}

ModMenuModule::MouseControlCheatMode ModMenuModule::MouseControlCheat::GetAutoModeTargetMode()
{
	CameraCheat* cameraCheat = CameraCheat::GetInstance();
	if (!cameraCheat->IsEnabled()) {
		return MouseControlCheatMode::PointAt;
	}

	CameraCheatOptions cameraOptions = cameraCheat->GetOptions();
	if (!cameraOptions.followPedRotation) {
		return MouseControlCheatMode::PointAt;
	}

	return MouseControlCheatMode::Rotate;
}

REGISTER_CHEAT(MouseControlCheat)
