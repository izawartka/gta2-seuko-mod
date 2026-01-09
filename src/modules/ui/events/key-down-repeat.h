#pragma once
#include "../common.h"
#include "../../../events/keyboard.h"

namespace UiModule {
	/*
	Dispatched when a key is pressed down. Repeats periodicaly after a longer press. Useful for inputing text.
	*/
	class KeyDownRepeatEvent : public KeyboardEvent {
	public:
		static bool Init();
		static void Deinit();
		KeyDownRepeatEvent(Game::KeyCode keyCode, bool shift = false, bool ctrl = false, bool alt = false)
			: KeyboardEvent(keyCode, shift, ctrl, alt) { }
	};
}
