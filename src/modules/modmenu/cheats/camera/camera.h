#pragma once
#include "../../common.h"
#include "../../cheat-base.h"
#include "camera-pos.h"
#include "../../../../events/game-start.h"
#include "../../../../events/draw-frame.h"
#include "../../../../events/renderer.h"
#include "../../../../events/draw-player-name.h"
#include "../../../../events/draw-map-layer.h"
#include "../../utils/vertex-utils.h"

namespace ModMenuModule {
	using CameraCheatFPROHandleId = size_t;

	struct CameraCheatOptions {
		Utils::Vertex::CameraTransform cameraTransform = {};
		bool customCulling = true;
		bool customRenderQueue = true;
		bool followPedRotation = false;
		float unused1 = 0.0f;
		size_t renderDistance = 20; // in map blocks, requires customRenderQueue = true
		float followPedRotationOffset = 0.0f; // requires followPedRotation = true
		bool autoHorRotCenter = true;

		bool SmartEquals(const CameraCheatOptions& other, bool ignoreFollowPedRotationOffset = false) const {
			if (!cameraTransform.SmartEquals(other.cameraTransform, followPedRotation, autoHorRotCenter)) return false;

			return customCulling == other.customCulling &&
				customRenderQueue == other.customRenderQueue &&
				followPedRotation == other.followPedRotation &&
				autoHorRotCenter == other.autoHorRotCenter && 
				(
					!followPedRotation ||
					ignoreFollowPedRotationOffset || 
					followPedRotationOffset == other.followPedRotationOffset
				) && (
					!customRenderQueue || 
					renderDistance == other.renderDistance
				);
		}

		bool operator==(const CameraCheatOptions& other) const {
			return cameraTransform == other.cameraTransform &&
				customCulling == other.customCulling &&
				customRenderQueue == other.customRenderQueue &&
				followPedRotation == other.followPedRotation &&
				followPedRotationOffset == other.followPedRotationOffset &&
				renderDistance == other.renderDistance &&
				autoHorRotCenter == other.autoHorRotCenter;
		}

		bool operator!=(const CameraCheatOptions& other) const {
			return !(*this == other);
		}
	};

	class CameraCheat : public CheatBase, public Core::EventListenerSupport {
	public:
		using OptionsT = CameraCheatOptions;
		CameraCheat();
		virtual ~CameraCheat() override;
		static CameraCheat* GetInstance();

		static auto GetDependencies() {
			return std::array<std::type_index, 1> { typeid(CameraPosCheat) };
		}

		void SetOptions(const CameraCheatOptions& options);
		const CameraCheatOptions& GetOptions() const { return m_options; }

		void SnapVerticalRotation();

		bool IsFPROManaged() const { return m_isFPROManaged; }
		CameraCheatFPROHandleId CreateFPROHandle();
		bool IsFPROHandleValid(CameraCheatFPROHandleId handleId) const;
		bool SetFPRO(CameraCheatFPROHandleId handleId, float value);
		void FreeFPROHandle(CameraCheatFPROHandleId handleId);

	private:
		virtual void OnFirstEnable() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

		void OnGameStart(GameStartEvent& event);
		void OnRendererLoad(RendererLoadEvent& event);
		void OnDrawTile(RendererDrawTileEvent& event);
		void OnDrawQuad(RendererDrawQuadEvent& event);
		void OnDrawTriangle(RendererDrawTriangleEvent& event);
		void OnDrawPlayerName(DrawPlayerNameEvent& event);
		void OnPreDrawFrame(PreDrawFrameEvent& event);
		void OnPreDrawMapLayer(PreDrawMapLayerEvent& event);

		void RequestAddCameraListeners();
		void AddCameraListeners();
		void RemoveCameraListeners();
		void UpdatePreDrawMapLayerListener();
		std::optional<float> GetAutoHorRotCenter(Game::Ped* cameraPed);
		std::optional<float> GetVerticalRotation(Game::Ped* cameraPed);
		void InternalFreeFPROHandle(bool useSetOptions);

		void SaveToPersistence() const;
		void LoadFromPersistence();
		static bool ConvertPersistence(std::unique_ptr<uint8_t[]>& dataPtr, size_t& dataSize, uint8_t version);

		static CameraCheat* m_instance;
		CameraCheatOptions m_options;
		bool m_hasRendererListeners = false;
		bool m_snapVerticalRotation = false;
		Game::GTAVertex m_vertexBuffer[8] = {};
		std::optional<Utils::Vertex::CameraValues> m_cameraValues = std::nullopt;
		std::optional<Utils::Vertex::CustomCameraPos> m_customCameraPos = std::nullopt;
		std::optional<Utils::Vertex::CachedCameraTransform> m_cachedCameraTransform = std::nullopt;
		std::vector<std::array<int, 2>> m_customRenderQueue = {};
		CameraCheatFPROHandleId m_currentFPROHandleId = 0;
		bool m_isFPROManaged = false;
		bool m_allowOneFPROEdit = false;
		float m_prevFPROValue = 0.0f;
	};
}
