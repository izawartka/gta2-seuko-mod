#pragma once
#include "../common.h"
#include "../cheat-base.h"
#include "../../../events/keyboard-get-data.h"
#include "../../../events/game-start.h"
#include "../../../events/game-end.h"
#include "../../../events/game-pause.h"
#include "../../../events/game-unpause.h"
#include "../../../events/draw-frame.h"

namespace ModMenuModule {
	enum class ForceControlState {
		ForceUp,
		Unmodified,
		ForceDown
	};

	/*
	The ForceControlsCheat allows forcing specific controls (mapped keys) to be pressed or preventing them from being pressed by the user.
	The cheat automatically enables / disables when needed, so SetEnabled should not be called directly.
	Controls are not modified when the game is not running, paused, or when replay is playing.
	The cheat should not be used for controls that are binded to a gamepad. Once the game starts, use CheckUsesGamepad to check.
	No state is saved to persistence.
	*/
	class ForceControlsCheat : public CheatBase, public Core::EventListenerSupport {
	public:
		using ControlIndex = size_t;
		using ControlHandle = size_t;

		ForceControlsCheat();
		virtual ~ForceControlsCheat() override;
		static ForceControlsCheat* GetInstance();

		static bool CheckUsesGamepad(ControlIndex controlIndex);
		static bool CheckGameReadyToForce(bool ignoreGamePause = false);

		bool IsControlFree(ControlIndex controlIndex) const;
		ControlHandle CreateControlHandle(ControlIndex controlIndex);
		void FreeControlHandle(ControlHandle handle);

		void SetControlState(ControlHandle controlHandle, ForceControlState state);
		ForceControlState GetControlState(ControlHandle controlHandle) const;

	private:
		virtual void OnFirstEnable() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

		void OnKeyboardGetData(KeyboardGetDataEvent& event);
		void OnGameStart(GameStartEvent& event);
		void OnGameEnd(GameEndEvent& event);
		void OnGamePause(GamePauseEvent& event);
		void OnGameUnpause(GameUnpauseEvent& event);
		void OnDrawFrame(DrawFrameEvent& event);

		void Start();
		void End();
		bool CheckCheatNeeded() const;
		void PreProcessEvent(KeyboardGetDataEvent& event);
		bool ProcessControl(KeyboardGetDataEvent& event, ControlIndex controlIndex);

		struct Control {
			ControlHandle handle = static_cast<ControlHandle>(-1);
			bool unmodifiedIsDown = false;
			ForceControlState state = ForceControlState::Unmodified;
			ForceControlState nextState = ForceControlState::Unmodified;
		};

		static ForceControlsCheat* m_instance;

		std::array<Control, Game::Constants::CONTROLS_COUNT> m_controls = {};
		std::unordered_map<ControlHandle, ControlIndex> m_controlHandles = {};
		ControlHandle m_nextControlHandle = 1;
		bool m_started = false;
	};
};
