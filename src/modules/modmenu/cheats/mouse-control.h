#pragma once
#include "../common.h"
#include "../cheat-base.h"
#include "force-controls.h"
#include "../../../events/game-tick.h"
#include "../../../events/game-start.h"
#include "../../../events/game-end.h"
#include "../../../events/game-pause.h"
#include "../../../events/game-unpause.h"
#include "../events/cheat-state.h"
#include "../events/cheat-options-update.h"
#include "../../mouse/normalized-mouse-position.h"

namespace ModMenuModule {
	class CameraCheat;

	enum class MouseControlCheatMode {
		Rotate = 0,
		PointAt = 2,
	};

	struct MouseControlCheatOptions {
		bool autoMode = true;
		MouseControlCheatMode mode = MouseControlCheatMode::Rotate;
		float rotateModeSensitivity = 0.02f;
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

	private:
		virtual void OnFirstEnable() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

		void OnPreGameTick(PreGameTickEvent& event);
		void OnMouseLockedMove(MouseModule::MouseLockedMoveEvent& event);
		void OnMouseMove(MouseModule::MouseMoveEvent& event);
		void OnMouseButtonDown(MouseModule::MouseButtonDownEvent& event);
		void OnMouseButtonUp(MouseModule::MouseButtonUpEvent& event);
		void OnGameStart(GameStartEvent& event);
		void OnGameEnd(GameEndEvent& event);
		void OnGamePause(GamePauseEvent& event);
		void OnGameUnpause(GameUnpauseEvent& event);
		void OnCheatStateChange(CheatStateEvent& event);
		void OnCameraCheatOptionsUpdate(CheatOptionsUpdateEvent<CameraCheat>& event);

		void SetOptionsInternal(const MouseControlCheatOptions& options);
		void UpdateTargetDeltaRotation();
		void UpdateMode();
		void UpdateLastNormalizedPos();
		void UpdateAttack() const;
		void UpdateAutoMode();
		void UpdateAutoModeListeners();
		void RemoveAutoModeListeners();
		bool EnsureNotGamepadControls() const;
		bool EnsureControlHandlesOk() const;
		void StartRotation();
		void StopRotation();
		void Start();
		void Stop();
		bool CreateControlHandles();
		void FreeControlHandles();

		void SaveToPersistence() const;
		void LoadFromPersistence();

		static bool CheckShouldUseRotation();
		static char GetRotationDirection(float deltaAngle);
		static std::optional<float> GetPlayerRotation();
		static std::optional<float> GetTargetRotation(MouseModule::NormalizedMousePosition normalizedPos);
		static MouseControlCheatMode GetAutoModeTargetMode();

		static MouseControlCheat* m_instance;
		MouseControlCheatOptions m_options;
		float m_targetDeltaRotation = 0.0f;
		MouseModule::NormalizedMousePosition m_lastNormalizedPos = {};
		bool m_started = false;
		ForceControlsCheat::ControlHandle m_leftControlHandle = -1;
		ForceControlsCheat::ControlHandle m_rightControlHandle = -1;
		ForceControlsCheat::ControlHandle m_attackControlHandle = -1;
		bool m_controlHandlesOk = false;
		bool m_usingRotation = false;
	};
}
