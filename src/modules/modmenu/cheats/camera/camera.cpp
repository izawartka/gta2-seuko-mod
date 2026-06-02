#include "camera.h"
#include "../../utils/angle-utils.h"
#include "../../utils/custom-render-queue-utils.h"
#include "../../utils/get-ped-position.h"
#include "../../utils/get-ped-rotation.h"
#include "../../../../events/culling-check.h"
#include "../../events/cheat-options-update.h"
#include "../../cheat-registry.h"

static constexpr size_t PERSISTENCE_VERSION = 3;

ModMenuModule::CameraCheat* ModMenuModule::CameraCheat::m_instance = nullptr;

ModMenuModule::CameraCheat::CameraCheat() : ModMenuModule::CheatBase("Cheat_Camera_IsEnabled") {
	assert(m_instance == nullptr && "CameraCheat instance already exists");
	m_instance = this;
}

ModMenuModule::CameraCheat::~CameraCheat()
{
	m_instance = nullptr;
}

ModMenuModule::CameraCheat* ModMenuModule::CameraCheat::GetInstance()
{
	assert(m_instance && "CameraCheat not initialized!");
	return m_instance;
}

void ModMenuModule::CameraCheat::SetOptions(const CameraCheatOptions& options)
{
	if (!IsEnabled()) {
		spdlog::warn("CameraCheat::SetOptions: Cheat is not enabled, cannot set options");
		return;
	}

	CameraCheatOptions oldOptions = m_options;
	m_options = options;

	if(!m_options.followPedRotation) {
		m_snapVerticalRotation = false;
		if(m_isFPROManaged) InternalFreeFPROHandle(false);
	}

	if (m_isFPROManaged && !m_allowOneFPROEdit && m_options.followPedRotationOffset != oldOptions.followPedRotationOffset) {
		spdlog::warn("CameraCheat::SetOptions: Tried to edit followPedRotationOffset, but it is managed");
		m_options.followPedRotationOffset = oldOptions.followPedRotationOffset;
	}
	m_allowOneFPROEdit = false;

	if (m_options.customRenderQueue != oldOptions.customRenderQueue) {
		UpdatePreDrawMapLayerListener();
	}

	if (m_options.cameraTransform != oldOptions.cameraTransform) {
		m_cachedCameraTransform = std::nullopt;
	}

	Core::EventManager* eventManager = Core::EventManager::GetInstance();
	CheatOptionsUpdateEvent<CameraCheat> event(oldOptions, m_options);
	eventManager->Dispatch(event);
}

void ModMenuModule::CameraCheat::SnapVerticalRotation()
{
	if (!IsEnabled()) {
		spdlog::warn("CameraCheat::SnapVerticalRotation: Cheat is not enabled, cannot snap vertical rotation");
		return;
	}

	if (!m_options.followPedRotation) {
		spdlog::warn("CameraCheat::SnapVerticalRotation: followPedRotation is disabled, cannot snap vertical rotation");
		return;
	}

	m_snapVerticalRotation = true;
}

ModMenuModule::CameraCheatFPROHandleId ModMenuModule::CameraCheat::CreateFPROHandle()
{
	if (!IsEnabled()) {
		spdlog::warn("CameraCheat::CreateFPROHandle: Cheat is not enabled, cannot create FPRO handle");
		return -1;
	}

	if (m_isFPROManaged) return -1;
	if (!m_options.followPedRotation) return -1;

	m_currentFPROHandleId++;
	if (m_currentFPROHandleId == -1) m_currentFPROHandleId = 1;

	m_isFPROManaged = true;
	m_prevFPROValue = m_options.followPedRotationOffset;

	return m_currentFPROHandleId;
}

bool ModMenuModule::CameraCheat::IsFPROHandleValid(CameraCheatFPROHandleId handleId) const
{
	if (!IsEnabled()) {
		spdlog::warn("CameraCheat::SnapVerticalRotation: Cheat is not enabled, cannot check FPRO handle");
		return false;
	}

	if (!m_isFPROManaged) return false;
	if (handleId != m_currentFPROHandleId) return false;
	if (!m_options.followPedRotation) return false;

	return true;
}

bool ModMenuModule::CameraCheat::SetFPRO(CameraCheatFPROHandleId handleId, float value)
{
	if (!IsEnabled()) {
		spdlog::warn("CameraCheat::SnapVerticalRotation: Cheat is not enabled, cannot set FPRO");
		return false;
	}

	if (!m_isFPROManaged) return false;
	if (handleId != m_currentFPROHandleId) return false;

	m_allowOneFPROEdit = true;
	CameraCheatOptions newOptions = m_options;
	newOptions.followPedRotationOffset = value;
	SetOptions(newOptions);

	return true;
}

void ModMenuModule::CameraCheat::FreeFPROHandle(CameraCheatFPROHandleId handleId)
{
	if (!IsEnabled()) {
		spdlog::warn("CameraCheat::SnapVerticalRotation: Cheat is not enabled, cannot free FPRO handle");
		return;
	}

	if (handleId != m_currentFPROHandleId) return;
	if (!m_isFPROManaged) return;

	InternalFreeFPROHandle(true);
}

void ModMenuModule::CameraCheat::OnFirstEnable()
{
	LoadFromPersistence();
}

void ModMenuModule::CameraCheat::OnEnable()
{
	RequestAddCameraListeners();
	UpdatePreDrawMapLayerListener();
}

void ModMenuModule::CameraCheat::OnDisable()
{
	RemoveCameraListeners();
	InternalFreeFPROHandle(false);
	SaveToPersistence();
	m_cameraValues = std::nullopt;
	m_customCameraPos = std::nullopt;
	m_cachedCameraTransform = std::nullopt;
	m_customRenderQueue.clear();
	m_snapVerticalRotation = false;
}

void ModMenuModule::CameraCheat::OnGameStart(GameStartEvent& event)
{
	if(IsEnabled() && m_options.followPedRotation) {
		m_snapVerticalRotation = true;
	}
}

void ModMenuModule::CameraCheat::OnRendererLoad(RendererLoadEvent& event)
{
	AddCameraListeners();
}

void ModMenuModule::CameraCheat::OnDrawTile(RendererDrawTileEvent& event)
{
	if (!m_cameraValues.has_value()) return;

	Utils::Vertex::ApplyQuadCameraTransform(
		event.GetVertices(),
		m_cameraValues.value(),
		m_options.cameraTransform
	);

	if (!m_options.customCulling) return;

	Utils::Vertex::ApplyCustomCulling(
		event.GetVertices(),
		4,
		m_cameraValues.value()
	);
}

void ModMenuModule::CameraCheat::OnDrawQuad(RendererDrawQuadEvent& event)
{
	if (!m_cameraValues.has_value()) return;
	if ((event.GetFlags() & 0x20000) != 0) return; // do not affect UI

	// car lights bug workaround
	memcpy(m_vertexBuffer, event.GetVertices(), sizeof(m_vertexBuffer));
	event.SetVertices(m_vertexBuffer);

	if (event.GetFlags() == 0xa180 && m_vertexBuffer[0].z < 0.01f) { // mission arrows and exploding scores
		Utils::Vertex::ApplyArrowsCameraTransform(
			m_vertexBuffer,
			m_cameraValues.value(),
			m_options.cameraTransform
		);

		return;
	}

	Utils::Vertex::ApplyQuadCameraTransform(
		m_vertexBuffer,
		m_cameraValues.value(),
		m_options.cameraTransform
	);

	if (!m_options.customCulling) return;

	Utils::Vertex::ApplyCustomCulling(
		m_vertexBuffer,
		4,
		m_cameraValues.value()
	);
}

void ModMenuModule::CameraCheat::OnDrawTriangle(RendererDrawTriangleEvent& event)
{
	if (!m_cameraValues.has_value()) return;

	bool isReversed;
	Utils::Vertex::ApplyTriangleCameraTransform(
		event.GetVertices(),
		m_cameraValues.value(),
		m_options.cameraTransform,
		&isReversed
	);

	if (!m_options.customCulling) return;

	Utils::Vertex::ApplyCustomCulling(
		event.GetVertices(),
		3,
		m_cameraValues.value(),
		isReversed
	);
}

void ModMenuModule::CameraCheat::OnDrawPlayerName(DrawPlayerNameEvent& event)
{
	if (!m_cameraValues.has_value()) return;

	Game::SCR_Vector3 position = event.GetPosition();

	Utils::Vertex::ApplyWorldPointCameraTransform(
		position,
		m_cameraValues.value(),
		m_options.cameraTransform
	);

	event.SetModifiedPosition(position);
}

void ModMenuModule::CameraCheat::OnPreDrawFrame(PreDrawFrameEvent& event)
{
	Game::Player* player = Game::Utils::GetPlayer();
	if (!player) return;

	Game::Camera* mainCamera = &player->ph2;
	Game::Ped* cameraPed = mainCamera->ped;

	bool doUpdateOptions = false;
	float oldVerticalAngleRad = m_options.cameraTransform.verticalAngleRad;
	float updatedVerticalAngleRad = oldVerticalAngleRad;
	std::optional<float> newVerticalAngleRad = GetVerticalRotation(cameraPed);
	if (newVerticalAngleRad.has_value() && newVerticalAngleRad != oldVerticalAngleRad) {
		updatedVerticalAngleRad = m_snapVerticalRotation ?
			newVerticalAngleRad.value() :
			Utils::Angle::LerpAngle(oldVerticalAngleRad, newVerticalAngleRad.value(), m_options.followPedRotationLerpFactor);

		doUpdateOptions = true;
	}

	m_snapVerticalRotation = false;

	float oldHorRotCenter = m_options.cameraTransform.horRotCenter;
	std::optional<float> newHorRotCenter = GetAutoHorRotCenter(cameraPed);
	float updatedHorRotCenter = oldHorRotCenter;
	if (newHorRotCenter.has_value() && newHorRotCenter != oldHorRotCenter) {
		updatedHorRotCenter = newHorRotCenter.value();

		doUpdateOptions = true;
	}

	if (doUpdateOptions) {
		CameraCheatOptions newOptions = m_options;
		newOptions.cameraTransform.verticalAngleRad = updatedVerticalAngleRad;
		newOptions.cameraTransform.horRotCenter = updatedHorRotCenter;
		SetOptions(newOptions);
	}

	if(!m_cachedCameraTransform.has_value()) {
		m_cachedCameraTransform = Utils::Vertex::CachedCameraTransform(m_options.cameraTransform);
	}

	m_cameraValues = Utils::Vertex::GetCameraValues(*mainCamera, m_cachedCameraTransform.value());
	m_customCameraPos = Utils::Vertex::GetCustomCameraPos(
		m_cameraValues.value(),
		m_cachedCameraTransform.value()
	);

	if (m_options.customRenderQueue && m_customCameraPos.has_value()) {
		m_customRenderQueue = Utils::CustomRenderQueue::BuildRenderQueue(
			Utils::CustomRenderQueue::GetQueueData(
				m_cameraValues.value(),
				m_customCameraPos.value(),
				m_options.renderDistance
			)
		);
	}
	else {
		m_customRenderQueue.clear();
	}
}

void ModMenuModule::CameraCheat::OnPreDrawMapLayer(PreDrawMapLayerEvent& event)
{
	if (!m_customRenderQueue.size()) return;

	event.SetSkipDraw(true);

	for (auto& pos : m_customRenderQueue) {
		Game::Functions::DrawTileBlock(event.GetS12(), 0, &pos[0], &pos[1]);
	}
}

void ModMenuModule::CameraCheat::RequestAddCameraListeners()
{
	AddEventListener<GameStartEvent>(&ModMenuModule::CameraCheat::OnGameStart);
	AddEventListener<PreDrawFrameEvent>(&ModMenuModule::CameraCheat::OnPreDrawFrame);

	if(!*Game::Memory::GetIsRendererLoaded()) {
		AddEventListener<RendererLoadEvent>(&ModMenuModule::CameraCheat::OnRendererLoad, true);
		return;
	}

	AddCameraListeners();
}

void ModMenuModule::CameraCheat::AddCameraListeners()
{
	if(!*Game::Memory::GetIsRendererLoaded()) {
		spdlog::error("CameraCheat::AddCameraListeners called but renderer is not loaded");
		return;
	}

	AddEventListener<RendererDrawTileEvent>(&ModMenuModule::CameraCheat::OnDrawTile);
	AddEventListener<RendererDrawQuadEvent>(&ModMenuModule::CameraCheat::OnDrawQuad);
	AddEventListener<RendererDrawTriangleEvent>(&ModMenuModule::CameraCheat::OnDrawTriangle);
	AddEventListener<DrawPlayerNameEvent>(&ModMenuModule::CameraCheat::OnDrawPlayerName);
	m_hasRendererListeners = true;
}

void ModMenuModule::CameraCheat::RemoveCameraListeners()
{
	RemoveEventListener<GameStartEvent>();
	RemoveEventListener<PreDrawFrameEvent>();
	RemoveEventListener<CullingCheckEvent>(true);
	RemoveEventListener<PreDrawMapLayerEvent>(true);

	if (m_hasRendererListeners) {
		RemoveEventListener<RendererDrawTileEvent>();
		RemoveEventListener<RendererDrawQuadEvent>();
		RemoveEventListener<RendererDrawTriangleEvent>();
		RemoveEventListener<DrawPlayerNameEvent>();
		m_hasRendererListeners = false;
	}

	RemoveEventListener<RendererLoadEvent>(true);
}

void ModMenuModule::CameraCheat::UpdatePreDrawMapLayerListener()
{
	SetEventListener<PreDrawMapLayerEvent>(&ModMenuModule::CameraCheat::OnPreDrawMapLayer, m_options.customRenderQueue);
}

std::optional<float> ModMenuModule::CameraCheat::GetAutoHorRotCenter(Game::Ped* cameraPed)
{
	if (!m_options.autoHorRotCenter) return std::nullopt;

	const CameraPosCheat* cameraPosCheat = CameraPosCheat::GetInstance();
	if (cameraPosCheat->IsEnabled() && cameraPosCheat->GetOptions().z.mode == CameraPosCheatMode::LockTargetAt) return std::nullopt;

	const Game::SCR_Vector3* pedPosition = Utils::GetPedPosition(cameraPed);
	if (!pedPosition) return std::nullopt;

	return Game::Utils::ToFloat(pedPosition->z);
}

std::optional<float> ModMenuModule::CameraCheat::GetVerticalRotation(Game::Ped* cameraPed)
{
	if (!m_options.followPedRotation) return std::nullopt;

	const Game::ushort* pedRotation = Utils::GetPedRotation(cameraPed);
	if (!pedRotation) return 0.0f;

	float pedRotationRad = Game::Utils::FromGTAAngleToRad(*pedRotation) + static_cast<float>(M_PI);
	return Utils::Angle::NormalizeAngle(pedRotationRad + m_options.followPedRotationOffset);
}

void ModMenuModule::CameraCheat::InternalFreeFPROHandle(bool useSetOptions)
{
	m_isFPROManaged = false;
	m_allowOneFPROEdit = false;

	if (useSetOptions && m_options.followPedRotationOffset != m_prevFPROValue) {
		CameraCheatOptions newOptions = m_options;
		newOptions.followPedRotationOffset = m_prevFPROValue;
		SetOptions(newOptions);
	}
	else {
		m_options.followPedRotationOffset = m_prevFPROValue;
	}

	m_prevFPROValue = 0.0f;
}

void ModMenuModule::CameraCheat::SaveToPersistence() const
{
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();
	size_t dataSize = 1 + sizeof(CameraCheatOptions);
	std::unique_ptr<uint8_t[]> dataPtr = std::make_unique<uint8_t[]>(dataSize);

	dataPtr[0] = PERSISTENCE_VERSION;
	memcpy(dataPtr.get() + 1, &m_options, sizeof(CameraCheatOptions));

	persistence->SaveRaw("Cheat_Camera_State", dataPtr.get(), dataSize);
}

void ModMenuModule::CameraCheat::LoadFromPersistence()
{
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();
	std::unique_ptr<uint8_t[]> dataPtr = nullptr;
	size_t dataSize = 0;
	if (!persistence->LoadRaw("Cheat_Camera_State", dataPtr, dataSize)) return;
	if (dataSize < 1) {
		spdlog::error("CameraCheat::LoadFromPersistence: invalid data size");
		return;
	}
	uint8_t version = dataPtr[0];
	if (version != PERSISTENCE_VERSION && !ConvertPersistence(dataPtr, dataSize, version)) {
		return;
	}
	if (dataSize != 1 + sizeof(CameraCheatOptions)) {
		spdlog::error("CameraCheat::LoadFromPersistence: invalid data size for version {}", version);
		return;
	}
	memcpy(&m_options, dataPtr.get() + 1, sizeof(CameraCheatOptions));
	SetOptions(m_options);
}

bool ModMenuModule::CameraCheat::ConvertPersistence(std::unique_ptr<uint8_t[]>& dataPtr, size_t& dataSize, uint8_t version)
{
	switch (version) {
	case 1: {
		// add float followPedRotationOffset at the end (equals 0)
		size_t newSize = dataSize + 4;
		std::unique_ptr<uint8_t[]> newDataPtr = std::make_unique<uint8_t[]>(newSize);
		memcpy(newDataPtr.get(), dataPtr.get(), dataSize);
		float followPedRotationOffset = 0.0f;
		memcpy(newDataPtr.get() + dataSize, &followPedRotationOffset, 4);
		dataPtr = std::move(newDataPtr);
		dataSize = newSize;
	} [[fallthrough]];
	case 2: {
		// add autoHorRotCenter and horRotCenter
		size_t newSize = dataSize + 8;
		std::unique_ptr<uint8_t[]> newDataPtr = std::make_unique<uint8_t[]>(newSize);
		memcpy(newDataPtr.get(), dataPtr.get(), dataSize);
		memcpy(newDataPtr.get() + 20, dataPtr.get() + 16, dataSize - 16);
		float horRotCenter = 2.0f;
		memcpy(newDataPtr.get() + 16, &horRotCenter, 4);
		bool autoHorRotCenter = true;
		memcpy(newDataPtr.get() + dataSize, &autoHorRotCenter, 1);
		dataPtr = std::move(newDataPtr);
		dataSize = newSize;
		return true;
	}
	default: {
		spdlog::error("CameraCheat::ConvertPersistence: unsupported version {}", version);
		return false;
	}}
}

REGISTER_CHEAT(CameraCheat)
