#pragma once
#include "../common.h"

namespace UiModule {
	/*
	Dispatched when a key is pressed down. Repeats periodicaly after a longer press. Useful for inputting text.
	*/
	class KeyDownRepeatEvent : public KeyboardModule::KeyboardEventBase {
	public:
		static bool Init();
		static void Deinit();
		KeyDownRepeatEvent(Game::KeyCode keyCode, bool shift = false, bool ctrl = false, bool alt = false, bool capsLock = false)
			: KeyboardModule::KeyboardEventBase(keyCode, shift, ctrl, alt, capsLock) { }
	};
}
