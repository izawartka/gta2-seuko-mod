#pragma once
#include "../../common.h"
#include "../../../../events/update-car-camera.h"
#include "../../../../events/camera-follow-update.h"
#include "../../../../events/game-end.h"
#include "../../cheat-base.h"

namespace ModMenuModule {
	struct SmoothCameraCheatOptions {
		bool smoothCarCamera = true;
		float carCameraHorMoveScale = 0.6f;
		float carCameraVertMoveScale = 0.9f;
		float carCameraMagLerpCoef = 0.3f;
		Game::SCR_f carCameraMagMaxDelta = Game::Utils::FromFloat(0.25f);
		float carCameraHorLerpCoef = 0.5f;
		float carCameraHorMaxDelta = 0.25f;
		float carCameraVertLerpCoef = 0.6f;
		float carCameraVertMaxDelta = 0.3f;
		bool smoothFollowCamera = true;
		float followLerpCoef = 0.3f;
		Game::SCR_f followMaxDelta = Game::Utils::FromFloat(0.8f);
		Game::SCR_f followZoomMaxDelta = Game::Utils::FromFloat(0.1f);
	};

	class SmoothCameraCheat : public CheatBase, public Core::EventListenerSupport {
	public:
		using OptionsT = SmoothCameraCheatOptions;
		SmoothCameraCheat();
		virtual ~SmoothCameraCheat() override;
		static SmoothCameraCheat* GetInstance();

		void SetOptions(const SmoothCameraCheatOptions& options);
		const SmoothCameraCheatOptions& GetOptions() const { return m_options; }

	private:
		virtual void OnFirstEnable() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

		void OnPreUpdateCarCamera(PreUpdateCarCameraEvent& event);
		void OnPostUpdateCarCamera(PostUpdateCarCameraEvent& event);
		void OnCameraFollowUpdate(CameraFollowUpdateEvent& event);
		void OnGameEnd(GameEndEvent& event);

		void UpdateSmoothCarCameraListeners();
		void UpdateSmoothFollowCameraListeners();

		void SaveToPersistence() const;
		void LoadFromPersistence();
		static bool ConvertPersistence(std::unique_ptr<uint8_t[]>& dataPtr, size_t& dataSize, uint8_t version);

		static SmoothCameraCheat* m_instance;
		SmoothCameraCheatOptions m_options = {};
		Game::Car* m_currentCar = nullptr;
		std::optional<Game::SCR_Vector3> m_lastCarDelta = std::nullopt;
		std::optional<Game::SCR_Vector3> m_preCameraTargetPos = std::nullopt;
	};
}
