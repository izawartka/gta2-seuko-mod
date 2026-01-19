#include "camera.h"
#include "camera-pos.h"
#include "../../cheat-registry.h"
#include "../../utils/angle-utils.h"
#include "../../utils/custom-render-queue-utils.h"
#include "../../../../events/culling-check.h"
#include "../../root.h"

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

	m_options = options;
	if(!m_options.followPedRotation) {
		m_snapVerticalRotation = false;
	}
	UpdatePreDrawMapLayerListener();
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
	SaveToPersistence();
	RemoveCameraListeners();
	m_cameraValues = std::nullopt;
	m_customCameraPos = std::nullopt;
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

void ModMenuModule::CameraCheat::OnPreDrawFrame(PreDrawFrameEvent& event)
{
	Game::Player* player = Core::MakeResolver(Game::Memory::GetGame, mem(&Game::Game::CurrentPlayer))();
	if (!player) return;

	Game::Ped* playerPed = player->ped;
	short* playerPedRotationPtr = Game::Utils::GetPlayerPedRotationPtr();
	if (playerPedRotationPtr != nullptr && m_options.followPedRotation) {
		float pedRotationRad = Game::Utils::FromGTAAngleToRad(*playerPedRotationPtr) + static_cast<float>(M_PI);
		float& cameraRotation = m_options.cameraTransform.verticalAngleRad;
		if (m_snapVerticalRotation) {
			cameraRotation = pedRotationRad;
		}
		else {
			cameraRotation = Utils::Angle::LerpAngle(cameraRotation, pedRotationRad, m_options.followPedRotationLerpFactor);
		}
	}

	m_snapVerticalRotation = false;

	auto* cameraPosCheat = CameraPosCheat::GetInstance();
	bool isZlocked = cameraPosCheat && cameraPosCheat->IsEnabled() && cameraPosCheat->GetOptions().z.mode == CameraPosCheatMode::LockTargetAt;
	bool ignorePlayerPedZ = !playerPed || isZlocked;
	Game::SCR_f playerPedZ = ignorePlayerPedZ ? Game::Utils::FromFloat(3.0f) : playerPed->position.z;

	Game::Camera* mainCamera = &player->ph2;
	m_cameraValues = Utils::Vertex::GetCameraValues(*mainCamera, playerPedZ);
	m_customCameraPos = Utils::Vertex::GetCustomCameraPos(
		m_cameraValues.value(),
		m_options.cameraTransform
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
	m_hasRendererListeners = true;
}

void ModMenuModule::CameraCheat::RemoveCameraListeners()
{
	RemoveEventListener<GameStartEvent>();
	RemoveEventListener<PreDrawFrameEvent>();

	if (HasEventListener<CullingCheckEvent>()) {
		RemoveEventListener<CullingCheckEvent>();
	}

	if (HasEventListener<PreDrawMapLayerEvent>()) {
		RemoveEventListener<PreDrawMapLayerEvent>();
	}

	if (m_hasRendererListeners) {
		RemoveEventListener<RendererDrawTileEvent>();
		RemoveEventListener<RendererDrawQuadEvent>();
		RemoveEventListener<RendererDrawTriangleEvent>();
		m_hasRendererListeners = false;
	}

	if (HasEventListener<RendererLoadEvent>()) {
		RemoveEventListener<RendererLoadEvent>();
	}
}

void ModMenuModule::CameraCheat::UpdatePreDrawMapLayerListener()
{
	SetEventListener<PreDrawMapLayerEvent>(&ModMenuModule::CameraCheat::OnPreDrawMapLayer, m_options.customRenderQueue);
}

void ModMenuModule::CameraCheat::SaveToPersistence() const
{
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();
	size_t dataSize = 1 + sizeof(CameraCheatOptions);
	std::unique_ptr<uint8_t[]> dataPtr = std::make_unique<uint8_t[]>(dataSize);

	dataPtr[0] = 1; // version
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
	if (version != 1) {
		spdlog::error("CameraCheat::LoadFromPersistence: unsupported version {}", version);
		return;
	}
	if (dataSize != 1 + sizeof(CameraCheatOptions)) {
		spdlog::error("CameraCheat::LoadFromPersistence: invalid data size for version {}", version);
		return;
	}
	memcpy(&m_options, dataPtr.get() + 1, sizeof(CameraCheatOptions));
	SetOptions(m_options);
}

REGISTER_CHEAT(CameraCheat)
