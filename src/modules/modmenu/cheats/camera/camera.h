#pragma once
#include "../../common.h"
#include "../../cheat-base.h"
#include "../../../../events/draw-frame.h"
#include "../../../../events/renderer.h"
#include "../../../../events/culling-check.h"
#include "../../../../events/draw-map-layer.h"
#include "../../utils/vertex-utils.h"

namespace ModMenuModule {
	struct CameraCheatOptions {
		Utils::Vertex::CameraTransform cameraTransform = {};
		bool customCulling = true;
		bool customRenderQueue = true;
		bool followPedRotation = false;
		float followPedRotationLerpFactor = 0.5f; // requires followPedRotation = true
		size_t renderDistance = 20; // in map blocks, requires customRenderQueue = true
	};

	class CameraCheat : public CheatBase, public Core::EventListenerSupport {
	public:
		CameraCheat();
		virtual ~CameraCheat();

		void SetOptions(const CameraCheatOptions& options);
		const CameraCheatOptions& GetOptions() const { return m_options; }

	private:
		virtual void OnFirstEnable() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

		void OnRendererLoad(RendererLoadEvent& event);
		void OnDrawTile(RendererDrawTileEvent& event);
		void OnDrawQuad(RendererDrawQuadEvent& event);
		void OnDrawTriangle(RendererDrawTriangleEvent& event);
		void OnPreDrawFrame(PreDrawFrameEvent& event);
		void OnPreDrawMapLayer(PreDrawMapLayerEvent& event);

		void RequestAddCameraListeners();
		void AddCameraListeners();
		void RemoveCameraListeners();
		void UpdatePreDrawMapLayerListener();

		void SaveToPersistence() const;
		void LoadFromPersistence();

		CameraCheatOptions m_options;
		bool m_hasRendererListeners = false;
		Game::GTAVertex m_vertexBuffer[8] = {};
		std::optional<Utils::Vertex::CameraValues> m_cameraValues = std::nullopt;
		std::optional<Utils::Vertex::CustomCameraPos> m_customCameraPos = std::nullopt;
		std::vector<std::array<int, 2>> m_customRenderQueue = {};
	};
}
