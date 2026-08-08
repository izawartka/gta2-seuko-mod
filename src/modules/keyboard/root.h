#pragma once
#include "common.h"
#include "../../events/keyboard-get-data.h"

namespace KeyboardModule {
	class KeyboardEvent;

	class RootModule : public Core::ModuleBase {
	public:
		RootModule();
		virtual ~RootModule() override;
		static RootModule* GetInstance();
		bool Attach();
		void Detach();

		bool IsKeyDown(Game::KeyCode keyCode) const;
		bool IsCapsLockOn() const;

	private:
		void OnKeyboardGetData(KeyboardGetDataEvent& event);
		void OnPostKeyboardGetData(PostKeyboardGetDataEvent& event);
		// @returns Previous isDown state
		bool SetKeyState(Game::KeyCode keyCode, bool isDown);
		void UpdateCapsLock();

		static RootModule* m_instance;
		bool m_keys[256] = { 0 };
		bool m_isCapsLockOn = false;
	};
}
