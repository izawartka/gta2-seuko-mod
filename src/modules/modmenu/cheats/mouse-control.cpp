#include "mouse-control.h"
#include "../utils/angle-utils.h"
#include "../events/cheat-options-update.h"
#include "camera/camera.h"
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

	MouseControlCheatOptions oldOptions = m_options;
	m_options = options;

	bool modeChanged = (oldOptions.mode != m_options.mode);
	if (m_started && modeChanged) UpdateMode();

	Core::EventManager* eventManager = Core::EventManager::GetInstance();
	CheatOptionsUpdateEvent<MouseControlCheat> event(oldOptions, m_options);
	eventManager->Dispatch(event);
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
	if (CheckShouldStart()) Start();
}

void ModMenuModule::MouseControlCheat::OnDisable()
{
	RemoveEventListener<GameStartEvent>();
	RemoveEventListener<GameEndEvent>();
	RemoveEventListener<GamePauseEvent>();
	RemoveEventListener<GameUnpauseEvent>();
	Stop();
	SaveToPersistence();
}

void ModMenuModule::MouseControlCheat::OnPreGameTick(PreGameTickEvent& event)
{
	UpdateTargetDeltaRotation();
}

void ModMenuModule::MouseControlCheat::OnKeyboardStateUpdate(KeyboardStateUpdateEvent& event)
{
	Game::Game* game = Game::Memory::GetGame();
	if (!game) return;
	Game::Player* player = game->CurrentPlayer;
	if (event.GetPlayer() != player) return;

	Game::KEYBOARD_STATE state = event.GetModifiedState();
	std::optional<MouseModule::MouseState> lastMouseStateOpt = MouseModule::MouseManager::GetInstance()->GetLastMouseState();
	if (!lastMouseStateOpt.has_value()) return;

	state = static_cast<Game::KEYBOARD_STATE>(state | (lastMouseStateOpt->leftButtonDown ? Game::KEYBOARD_STATE_ATTACK : 0));
	state = static_cast<Game::KEYBOARD_STATE>(state & ~(Game::KEYBOARD_STATE_ROTATE_LEFT | Game::KEYBOARD_STATE_ROTATE_RIGHT));

	Game::KEYBOARD_STATE inputState = GetRotationInput(m_targetDeltaRotation);
	if (inputState & Game::KEYBOARD_STATE_ROTATE_LEFT) m_targetDeltaRotation -= ROTATION_SPEED;
	else if (inputState & Game::KEYBOARD_STATE_ROTATE_RIGHT) m_targetDeltaRotation += ROTATION_SPEED;

	state = static_cast<Game::KEYBOARD_STATE>(state | inputState);

	event.SetModifiedState(state);
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

void ModMenuModule::MouseControlCheat::OnGameStart(GameStartEvent& event)
{
	if (CheckShouldStart()) Start();
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
	if (CheckShouldStart(true)) Start();
}

void ModMenuModule::MouseControlCheat::UpdateTargetDeltaRotation()
{
	if (!CheckShouldUseRotation()) {
		m_targetDeltaRotation = 0;
		return;
	}

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

void ModMenuModule::MouseControlCheat::Start()
{
	if (m_started) return;
	m_started = true;
	AddEventListener<PreGameTickEvent>(&MouseControlCheat::OnPreGameTick);
	AddEventListener<KeyboardStateUpdateEvent>(&MouseControlCheat::OnKeyboardStateUpdate);
	UpdateMode();
}

void ModMenuModule::MouseControlCheat::Stop()
{
	if (!m_started) return;
	m_started = false;
	RemoveEventListener<PreGameTickEvent>();
	RemoveEventListener<KeyboardStateUpdateEvent>();
	if (HasEventListener<MouseModule::MouseLockedMoveEvent>()) RemoveEventListener<MouseModule::MouseLockedMoveEvent>();
	if (HasEventListener<MouseModule::MouseMoveEvent>()) RemoveEventListener<MouseModule::MouseMoveEvent>();

	MouseModule::MouseManager* mouseManager = MouseModule::MouseManager::GetInstance();
	mouseManager->SetLocked(false);
	mouseManager->SetInvisible(false);

	m_targetDeltaRotation = 0.0f;
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

bool ModMenuModule::MouseControlCheat::CheckShouldStart(bool forceWithPause)
{
	Game::Game* game = Game::Memory::GetGame();
	if (!game) return false;

	Game::GAME_STATUS expectedStatus = forceWithPause ? Game::GAME_PAUSED : Game::GAME_RUN;
	if (game->gameStatus != expectedStatus) return false;

	Game::Keyboard* keyboard = Game::Memory::GetKeyboard();
	if (!keyboard) return false;

	return keyboard->replayStatus == Game::REPLAY_STATUS_NOT_PLAYING;
}

bool ModMenuModule::MouseControlCheat::CheckShouldUseRotation()
{
	Game::Game* game = Game::Memory::GetGame();
	if (!game) return false;
	Game::Player* player = game->CurrentPlayer;
	if (!player || !player->ped || !player->ped->gameObject) return false;
	if (player->ped->targetCarForEnter) return false;
	return true;
}

Game::KEYBOARD_STATE ModMenuModule::MouseControlCheat::GetRotationInput(float deltaAngle)
{
	Game::KEYBOARD_STATE direction = deltaAngle > 0 ? Game::KEYBOARD_STATE_ROTATE_LEFT : Game::KEYBOARD_STATE_ROTATE_RIGHT;
	if (std::abs(deltaAngle) < ROTATION_INPUT_THRESHOLD) {
		return Game::KEYBOARD_STATE(0);
	}
	return direction;
}

std::optional<float> ModMenuModule::MouseControlCheat::GetPlayerRotation()
{
	Game::Game* game = Game::Memory::GetGame();
	if (!game) return std::nullopt;
	Game::Player* player = game->CurrentPlayer;
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

REGISTER_CHEAT(MouseControlCheat)
