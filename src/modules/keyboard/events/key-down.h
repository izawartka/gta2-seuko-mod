#pragma once
#include "keyboard-base.h"

namespace KeyboardModule {
	/*
	Dispatched when a key is pressed down.
	*/
	class KeyDownEvent : public KeyboardEventBase {
	public:
		static bool Init();

		KeyDownEvent(Game::KeyCode keyCode, bool shift = false, bool ctrl = false, bool alt = false, bool capsLock = false)
			: KeyboardEventBase(keyCode, shift, ctrl, alt, capsLock) {}
	};
}
