#include "mouse-control.h"
#include "../camera/camera.h"
#include "../../toast-manager.h"
#include "../../events/cheat-options-update.h"
#include "../../cheat-registry.h"

static constexpr size_t PERSISTENCE_VERSION = 2;

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

	Core::EventManager* eventManager = Core::EventManager::GetInstance();
	CheatOptionsUpdateEvent<MouseControlCheat> event(oldOptions, m_options);
	eventManager->Dispatch(event);
}

void ModMenuModule::MouseControlCheat::ShowGamepadControlsToast()
{
	if (m_gamepadControlsToastShown) return;
	m_gamepadControlsToastShown = true;

	// the name "Controller" is purposely used instead of "Gamepad" or because that's how the preset is named in GTA2 Manager
	spdlog::warn("Mouse control won't work with Controller controls. Please update your controls in GTA2 Manager");
	ToastManager::GetInstance()->Show({ L"Mouse control won't work with Controller controls.", ToastType::Warning, 240 });
	ToastManager::GetInstance()->Show({ L"Please update your controls in GTA2 Manager", ToastType::Warning, 240 });
}


void ModMenuModule::MouseControlCheat::OnFirstEnable()
{
	LoadFromPersistence();
}

void ModMenuModule::MouseControlCheat::OnEnable()
{
	AddEventListener<PreGameTickEvent>(&MouseControlCheat::OnPreGameTick);
	AddEventListener<GameEndEvent>(&MouseControlCheat::OnGameEnd);
	AddEventListener<GamePauseEvent>(&MouseControlCheat::OnGamePause);
}

void ModMenuModule::MouseControlCheat::OnDisable()
{
	RemoveEventListener<PreGameTickEvent>();
	RemoveEventListener<GameEndEvent>();
	RemoveEventListener<GamePauseEvent>();

	ClearWorkerSet();
	SaveToPersistence();

	m_gamepadControlsToastShown = false;
}

void ModMenuModule::MouseControlCheat::OnPreGameTick(PreGameTickEvent& event)
{
	ApplyAutoMode();
	UpdateWorkerSet();
	SendWorkersUpdate();
}

void ModMenuModule::MouseControlCheat::OnGameEnd(GameEndEvent& event)
{
	ClearWorkerSet();
}

void ModMenuModule::MouseControlCheat::OnGamePause(GamePauseEvent& event)
{
	ClearWorkerSet();
}

void ModMenuModule::MouseControlCheat::ApplyAutoMode()
{
	if (!m_options.autoMode) return;

	MouseControlCheatOptions newOptions = m_options;
	newOptions.mode = GetAutoModeTargetMode();

	if (newOptions.mode != m_options.mode) {
		SetOptions(newOptions);
	}
}

ModMenuModule::MouseControlCheatMode ModMenuModule::MouseControlCheat::GetAutoModeTargetMode() const
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

void ModMenuModule::MouseControlCheat::UpdateWorkerSet()
{
	using namespace MouseControlWorkerRegistry;

	WorkerSetType appliableWorkerSetType = GetAppliableWorkerSetType();
	if (appliableWorkerSetType == m_workerSetType) return;
	if (appliableWorkerSetType == WorkerSetType::None) {
		ClearWorkerSet();
		return;
	}

	const MouseControlWorkerSetDef& workerSetDef = GetWorkerSetDef(appliableWorkerSetType);

	UpdateWorker(m_attackWorker, workerSetDef.attackWorkerType);
	UpdateWorker(m_mouseWorker, workerSetDef.mouseWorkerType);
	UpdateWorker(m_resultWorker, workerSetDef.resultWorkerType);
}

void ModMenuModule::MouseControlCheat::ClearWorkerSet()
{
	m_workerSetType = MouseControlWorkerRegistry::WorkerSetType::None;
	RemoveWorker(m_attackWorker);
	RemoveWorker(m_mouseWorker);
	RemoveWorker(m_resultWorker);
}

ModMenuModule::MouseControlWorkerRegistry::WorkerSetType ModMenuModule::MouseControlCheat::GetAppliableWorkerSetType() const
{
	using namespace MouseControlWorkerRegistry;

	Game::Game* game = Game::Memory::GetGame();
	if (!game || game->gameStatus != Game::GAME_RUN) {
		return WorkerSetType::None;
	}

	Game::Player* player = game->currentPlayer;
	Game::Ped* playerPed = player ? Game::Functions::GetCurrentPed(player) : nullptr;

	bool isAltMoving = player && player->keySpecial2;
	bool isFreecam = player && player->controlState == Game::PLAYER_CONTROL_STATE_FREECAM;
	bool isInCar = playerPed && (playerPed->currentCar != nullptr || playerPed->targetCarForEnter != nullptr);
	bool hasGameObject = playerPed && playerPed->gameObject != nullptr;

	switch (m_options.mode) {
	case MouseControlCheatMode::Rotate:
		if (isFreecam) return m_options.rotateCamera ? WorkerSetType::RotateModeFreecam : WorkerSetType::None;
		if (isAltMoving) return m_options.rotateCamera ? WorkerSetType::RotateModeAltMoving : WorkerSetType::AttackOnly;
		if (isInCar) return m_options.rotateCamera ? WorkerSetType::RotateModeInCar : WorkerSetType::AttackOnly;
		if (hasGameObject) return WorkerSetType::RotateMode;
		return WorkerSetType::None;
	case MouseControlCheatMode::PointAt:
		if (isFreecam) return WorkerSetType::None;
		if (isInCar || isAltMoving) return WorkerSetType::AttackOnly;
		if (hasGameObject) return WorkerSetType::PointAtMode;
		return WorkerSetType::None;
	default:
		spdlog::error("MouseControlCheat::GetAppliableWorkerSet: unknown mode {}", static_cast<int>(m_options.mode));
		return WorkerSetType::None;
	}
}

void ModMenuModule::MouseControlCheat::UpdateWorker(std::unique_ptr<MouseControlWorker>& worker, MouseControlWorkerRegistry::WorkerType targetType)
{
	if (worker && MouseControlWorkerRegistry::GetWorkerType(worker.get()) == targetType) {
		return;
	}

	if (worker) {
		RemoveWorker(worker);
	}

	if (targetType != MouseControlWorkerRegistry::GetWorkerType<void>()) {
		CreateWorker(worker, targetType);
	}
}

void ModMenuModule::MouseControlCheat::CreateWorker(std::unique_ptr<MouseControlWorker>& worker, MouseControlWorkerRegistry::WorkerType type)
{
	std::unique_ptr<MouseControlWorker> newWorker = MouseControlWorkerRegistry::CreateWorker(type);
	if (!newWorker) {
	   spdlog::error("MouseControlCheat::CreateWorker: failed to create worker for type {}", type.name());
	   return;
	}

	newWorker->Start();
	if(!newWorker->IsRunning()) {
		spdlog::error("MouseControlCheat::CreateWorker: failed to start worker for type {}", type.name());
		return;
	}

	worker = std::move(newWorker);
}

void ModMenuModule::MouseControlCheat::RemoveWorker(std::unique_ptr<MouseControlWorker>& worker)
{
	if (!worker) return;

	worker->Stop();
	worker.reset();
}

void ModMenuModule::MouseControlCheat::SendWorkersUpdate()
{
	if (m_attackWorker) m_attackWorker->Update();
	if (m_mouseWorker) m_mouseWorker->Update();
	if (m_resultWorker) m_resultWorker->Update();
}

void ModMenuModule::MouseControlCheat::SaveToPersistence() const
{
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();
	size_t dataSize = 1 + sizeof(MouseControlCheatOptions);
	std::unique_ptr<uint8_t[]> dataPtr = std::make_unique<uint8_t[]>(dataSize);

	dataPtr[0] = PERSISTENCE_VERSION;
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
	if (version != PERSISTENCE_VERSION) {
		if (!ConvertPersistence(dataPtr, dataSize, version)) return;
	}
	if (dataSize != 1 + sizeof(MouseControlCheatOptions)) {
		spdlog::error("MouseControlCheat::LoadFromPersistence: invalid persistence data size");
		return;
	}
	memcpy(&m_options, dataPtr.get() + 1, sizeof(MouseControlCheatOptions));
	SetOptions(m_options);
}

bool ModMenuModule::MouseControlCheat::ConvertPersistence(std::unique_ptr<uint8_t[]>& dataPtr, size_t& dataSize, uint8_t version)
{
	switch (version) {
	case 1: {
		// add rotateCamera option at the end
		size_t newSize = dataSize + sizeof(bool);
		std::unique_ptr<uint8_t[]> newDataPtr = std::make_unique<uint8_t[]>(newSize);
		memcpy(newDataPtr.get(), dataPtr.get(), dataSize);
		bool rotateCamera = true;
		memcpy(newDataPtr.get() + dataSize, &rotateCamera, sizeof(bool));
		dataPtr = std::move(newDataPtr);
		dataSize = newSize;
		return true;
	}
	default: {
		spdlog::error("MouseControlCheat::ConvertPersistence: unsupported version {}", version);
		return false;
	}}
}

REGISTER_CHEAT(MouseControlCheat)
