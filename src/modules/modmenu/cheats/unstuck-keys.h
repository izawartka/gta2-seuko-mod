#pragma once
#include "../common.h"
#include "../cheat-base.h"
#include "../../../events/window-focus-change.h"
#include "../../../events/keyboard-get-data.h"
#include "../../../events/game-start.h"

namespace ModMenuModule {
	class UnstuckKeysCheat : public CheatBase, public Core::EventListenerSupport {
	public:
		UnstuckKeysCheat();
		virtual ~UnstuckKeysCheat() override;
		static UnstuckKeysCheat* GetInstance();

	private:
		virtual void OnFirstEnable() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;
		virtual bool AutoEnableOnAttach() const override { return true; }

		void OnWindowFocusChange(WindowFocusChangeEvent& event);
		void OnGameStart(GameStartEvent& event);
		void OnKeyboardGetData(KeyboardGetDataEvent& event);
		void OnPostKeyboardGetData(PostKeyboardGetDataEvent& event);

		void StopUnstuck();

		static UnstuckKeysCheat* m_instance;

		std::unordered_set<Game::KeyCode> m_stuckKeys = {};
		std::unordered_set<Game::KeyCode> m_heldKeys = {};
		bool m_unstuckInProgress = false;
	};
}
