#pragma once
#include "keyboard-base.h"

namespace KeyboardModule {
	/*
	Dispatched when a key is released.
	*/
	class KeyUpEvent : public KeyboardEventBase {
	public:
		static bool Init();

		KeyUpEvent(Game::KeyCode keyCode, bool shift = false, bool ctrl = false, bool alt = false, bool capsLock = false)
			: KeyboardEventBase(keyCode, shift, ctrl, alt, capsLock) {}
	};
}
