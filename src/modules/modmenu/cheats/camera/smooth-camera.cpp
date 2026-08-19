#include "smooth-camera.h"
#include "../../events/cheat-options-update.h"
#include "../../cheat-registry.h"
#include "../../utils/scrf-lerp-utils.h"

static constexpr size_t PERSISTENCE_VERSION = 1;

ModMenuModule::SmoothCameraCheat* ModMenuModule::SmoothCameraCheat::m_instance = nullptr;

ModMenuModule::SmoothCameraCheat::SmoothCameraCheat() : ModMenuModule::CheatBase("Cheat_SmoothCamera_IsEnabled") {
	assert(m_instance == nullptr && "SmoothCameraCheat instance already exists");
	m_instance = this;
}

ModMenuModule::SmoothCameraCheat::~SmoothCameraCheat()
{
	m_instance = nullptr;
}

ModMenuModule::SmoothCameraCheat* ModMenuModule::SmoothCameraCheat::GetInstance()
{
	assert(m_instance && "SmoothCameraCheat not initialized!");
	return m_instance;
}

void ModMenuModule::SmoothCameraCheat::SetOptions(const SmoothCameraCheatOptions& options)
{
	if (!IsEnabled()) {
		spdlog::warn("SmoothCameraCheat::SetOptions: Cheat is not enabled, cannot set options");
		return;
	}

	SmoothCameraCheatOptions oldOptions = m_options;
	m_options = options;

	UpdateSmoothCarCameraListeners();
	UpdateSmoothFollowCameraListeners();

	Core::EventManager* eventManager = Core::EventManager::GetInstance();
	CheatOptionsUpdateEvent<SmoothCameraCheat> event(oldOptions, m_options);
	eventManager->Dispatch(event);
}

void ModMenuModule::SmoothCameraCheat::OnFirstEnable()
{
	LoadFromPersistence();
}

void ModMenuModule::SmoothCameraCheat::OnEnable()
{
	UpdateSmoothCarCameraListeners();
	UpdateSmoothFollowCameraListeners();
}

void ModMenuModule::SmoothCameraCheat::OnDisable()
{
	RemoveEventListener<PreUpdateCarCameraEvent>(true);
	RemoveEventListener<PostUpdateCarCameraEvent>(true);
	RemoveEventListener<CameraFollowUpdateEvent>(true);

	m_preCameraTargetPos = std::nullopt;
	m_lastCarDelta = std::nullopt;
	m_currentCar = nullptr;

	SaveToPersistence();
}

void ModMenuModule::SmoothCameraCheat::OnPreUpdateCarCamera(PreUpdateCarCameraEvent& event)
{
	Game::Player* player = Game::Utils::GetPlayer();
	Game::Camera* currentCamera = player ? Game::Functions::GetPlayerCurrentCamera(player) : nullptr;
	Game::Camera* camera = event.GetCamera();
	if (!camera || camera != currentCamera) return;

	Game::Car* currentCar = event.GetCar();
	if (currentCar != m_currentCar) {
		m_currentCar = currentCar;
		m_lastCarDelta = std::nullopt;
	}

	m_preCameraTargetPos = event.GetCameraPos();
}

void ModMenuModule::SmoothCameraCheat::OnPostUpdateCarCamera(PostUpdateCarCameraEvent& event)
{
	if (!m_preCameraTargetPos.has_value()) return;

	Game::Player* player = Game::Utils::GetPlayer();
	Game::Camera* currentCamera = player ? Game::Functions::GetPlayerCurrentCamera(player) : nullptr;
	Game::Camera* camera = event.GetCamera();
	if (!camera || camera != currentCamera) return;

	Game::SCR_Vector3 cameraPos = event.GetCameraPos();
	Game::SCR_Vector3 carDelta = cameraPos;
	Game::Utils::Sub(carDelta, m_preCameraTargetPos.value());
	carDelta.x = static_cast<Game::SCR_f>(carDelta.x * m_options.carCameraHorMoveScale + 0.5f);
	carDelta.y = static_cast<Game::SCR_f>(carDelta.y * m_options.carCameraHorMoveScale + 0.5f);
	carDelta.z = static_cast<Game::SCR_f>(carDelta.z * m_options.carCameraVertMoveScale + 0.5f);

	Game::SCR_Vector3 softCarDelta = m_lastCarDelta.value_or(carDelta);
	Utils::ScrfLerp::LerpAsAngleMag(
		softCarDelta, 
		carDelta,
		m_options.carCameraMagLerpCoef,
		m_options.carCameraMagMaxDelta,
		m_options.carCameraHorLerpCoef,
		m_options.carCameraHorMaxDelta,
		m_options.carCameraVertLerpCoef,
		m_options.carCameraVertMaxDelta
	);
	m_lastCarDelta = softCarDelta;

	Game::SCR_Vector3 newCameraPos = m_preCameraTargetPos.value();
	Game::Utils::Add(newCameraPos, softCarDelta);
	event.SetCameraPos(newCameraPos);

	m_preCameraTargetPos = std::nullopt;
}

void ModMenuModule::SmoothCameraCheat::OnCameraFollowUpdate(CameraFollowUpdateEvent& event)
{
	if (event.GetModifiedFollowState() != Game::CAMERA_FOLLOW_STATE_FOLLOW) return;
	Game::Player* player = Game::Utils::GetPlayer();
	Game::Camera* currentCamera = player ? Game::Functions::GetPlayerCurrentCamera(player) : nullptr;
	Game::Camera* camera = event.GetCamera();
	if (!camera || camera != currentCamera) return;

	Utils::ScrfLerp::Lerp(
		camera->cameraPos,
		camera->cameraPosTarget2,
		m_options.followLerpCoef,
		m_options.followMaxDelta,
		static_cast<int>(m_options.followZoomMaxDelta)
	);

	event.SetModifiedFollowState(Game::CAMERA_FOLLOW_STATE_NO_FOLLOW);
}

void ModMenuModule::SmoothCameraCheat::OnGameEnd(GameEndEvent& event)
{
	m_preCameraTargetPos = std::nullopt;
	m_lastCarDelta = std::nullopt;
	m_currentCar = nullptr;
}

void ModMenuModule::SmoothCameraCheat::UpdateSmoothCarCameraListeners()
{
	SetEventListener<PreUpdateCarCameraEvent>(&ModMenuModule::SmoothCameraCheat::OnPreUpdateCarCamera, m_options.smoothCarCamera);
	SetEventListener<PostUpdateCarCameraEvent>(&ModMenuModule::SmoothCameraCheat::OnPostUpdateCarCamera, m_options.smoothCarCamera);
	if (!m_options.smoothCarCamera) {
		m_preCameraTargetPos = std::nullopt;
		m_lastCarDelta = std::nullopt;
		m_currentCar = nullptr;
	}
}

void ModMenuModule::SmoothCameraCheat::UpdateSmoothFollowCameraListeners()
{
	SetEventListener<CameraFollowUpdateEvent>(&SmoothCameraCheat::OnCameraFollowUpdate, m_options.smoothFollowCamera);
}

void ModMenuModule::SmoothCameraCheat::SaveToPersistence() const
{
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();
	size_t dataSize = 1 + sizeof(SmoothCameraCheatOptions);
	std::unique_ptr<uint8_t[]> dataPtr = std::make_unique<uint8_t[]>(dataSize);

	dataPtr[0] = PERSISTENCE_VERSION;
	memcpy(dataPtr.get() + 1, &m_options, sizeof(SmoothCameraCheatOptions));

	persistence->SaveRaw("Cheat_SmoothCamera_State", dataPtr.get(), dataSize);
}

void ModMenuModule::SmoothCameraCheat::LoadFromPersistence()
{
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();
	std::unique_ptr<uint8_t[]> dataPtr = nullptr;
	size_t dataSize = 0;
	if (!persistence->LoadRaw("Cheat_SmoothCamera_State", dataPtr, dataSize)) return;
	if (dataSize < 1) {
		spdlog::error("SmoothCameraCheat::LoadFromPersistence: invalid data size");
		return;
	}
	uint8_t version = dataPtr[0];
	if (version != PERSISTENCE_VERSION && !ConvertPersistence(dataPtr, dataSize, version)) {
		return;
	}
	if (dataSize != 1 + sizeof(SmoothCameraCheatOptions)) {
		spdlog::error("SmoothCameraCheat::LoadFromPersistence: invalid data size for version {}", version);
		return;
	}
	memcpy(&m_options, dataPtr.get() + 1, sizeof(SmoothCameraCheatOptions));
	SetOptions(m_options);
}

bool ModMenuModule::SmoothCameraCheat::ConvertPersistence(std::unique_ptr<uint8_t[]>& dataPtr, size_t& dataSize, uint8_t version)
{
	spdlog::error("SmoothCameraCheat::ConvertPersistence: unsupported version {}", version);
	return false;
}

REGISTER_CHEAT(SmoothCameraCheat)
