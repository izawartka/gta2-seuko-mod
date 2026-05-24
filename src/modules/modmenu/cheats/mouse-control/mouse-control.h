#pragma once
#include "../../common.h"
#include "../../cheat-base.h"
#include "../force-controls.h"
#include "../../../../events/game-tick.h"
#include "../../../../events/game-end.h"
#include "../../../../events/game-pause.h"
#include "mouse-control-worker.h"
#include "mouse-control-worker-registry.h"

namespace ModMenuModule {
	enum class MouseControlCheatMode {
		Rotate = 0,
		PointAt = 2,
	};

	struct MouseControlCheatOptions {
		bool autoMode = true;
		MouseControlCheatMode mode = MouseControlCheatMode::Rotate;
		float rotateModeSensitivity = 0.02f;
		bool rotateCamera = true;
	};

	class MouseControlCheat : public CheatBase, public Core::EventListenerSupport {
	public:
		using OptionsT = MouseControlCheatOptions;
		MouseControlCheat();
		virtual ~MouseControlCheat() override;
		static MouseControlCheat* GetInstance();

		static auto GetDependencies() {
			return std::array<std::type_index, 1>{ typeid(ForceControlsCheat) };
		}

		static const std::vector<MouseControlCheatMode>& GetAllMouseControlCheatModes();

		void SetOptions(const MouseControlCheatOptions& options);
		MouseControlCheatOptions GetOptions() const { return m_options; }

		void ShowGamepadControlsToast();
		void SetDeltaX(float deltaX) { m_deltaX = deltaX; }
		float GetDeltaX() const { return m_deltaX; }
		void SetDeltaY(float deltaY) { m_deltaY = deltaY; }
		float GetDeltaY() const { return m_deltaY; }

	private:
		virtual void OnFirstEnable() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

		void OnPreGameTick(PreGameTickEvent& event);
		void OnGameEnd(GameEndEvent& event);
		void OnGamePause(GamePauseEvent& event);

		void ApplyAutoMode();
		MouseControlCheatMode GetAutoModeTargetMode() const;

		void UpdateWorkerSet();
		void ClearWorkerSet();
		MouseControlWorkerRegistry::WorkerSetType GetAppliableWorkerSetType() const;

		void UpdateWorker(std::unique_ptr<MouseControlWorker>& worker, MouseControlWorkerRegistry::WorkerType targetType);
		void CreateWorker(std::unique_ptr<MouseControlWorker>& worker, MouseControlWorkerRegistry::WorkerType type);
		void RemoveWorker(std::unique_ptr<MouseControlWorker>& worker);

		void SendWorkersUpdate();

		void SaveToPersistence() const;
		void LoadFromPersistence();
		static bool ConvertPersistence(std::unique_ptr<uint8_t[]>& dataPtr, size_t& dataSize, uint8_t version);

		static MouseControlCheat* m_instance;
		MouseControlCheatOptions m_options;
		MouseControlWorkerRegistry::WorkerSetType m_workerSetType = MouseControlWorkerRegistry::WorkerSetType::None;
		std::unique_ptr<MouseControlWorker> m_attackWorker;
		std::unique_ptr<MouseControlWorker> m_mouseWorker;
		std::unique_ptr<MouseControlWorker> m_resultWorker;
		bool m_gamepadControlsToastShown = false;

		float m_deltaX = 0.0f;
		float m_deltaY = 0.0f;
	};
}
