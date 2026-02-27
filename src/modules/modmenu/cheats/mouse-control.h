#pragma once
#include "../common.h"
#include "../cheat-base.h"
#include "../../../events/keyboard-state-update.h"
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

		static const std::vector<MouseControlCheatMode>& GetAllMouseControlCheatModes();

		void SetOptions(const MouseControlCheatOptions& options);
		MouseControlCheatOptions GetOptions() const { return m_options; }

	private:
		virtual void OnFirstEnable() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

		void OnPreGameTick(PreGameTickEvent& event);
		void OnKeyboardStateUpdate(KeyboardStateUpdateEvent& event);
		void OnMouseLockedMove(MouseModule::MouseLockedMoveEvent& event);
		void OnMouseMove(MouseModule::MouseMoveEvent& event);
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
		void UpdateAutoMode();
		void UpdateAutoModeListeners();
		void RemoveAutoModeListeners();
		void Start();
		void Stop();

		void SaveToPersistence() const;
		void LoadFromPersistence();

		static bool CheckShouldStart(bool forceWithPause = false);
		static bool CheckShouldUseRotation();
		static Game::KEYBOARD_STATE GetRotationInput(float deltaAngle);
		static std::optional<float> GetPlayerRotation();
		static std::optional<float> GetTargetRotation(MouseModule::NormalizedMousePosition normalizedPos);
		static MouseControlCheatMode GetAutoModeTargetMode();

		static MouseControlCheat* m_instance;
		float m_targetDeltaRotation = 0.0f;
		MouseModule::NormalizedMousePosition m_lastNormalizedPos = {};
		MouseControlCheatOptions m_options;
		bool m_started = false;
	};
}
