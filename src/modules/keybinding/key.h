#pragma once
#include "common.h"
#include "../../events/keyboard.h"

namespace KeyBindingModule {
	struct Key {
		Game::KeyCode keyCode = Game::KeyCode::DIK_NONE;
		bool shift = false;
		bool ctrl = false;
		bool alt = false;

		constexpr Key() = default;

		constexpr Key(Game::KeyCode keyCode, bool shift = false, bool ctrl = false, bool alt = false)
			: keyCode(keyCode), shift(shift), ctrl(ctrl), alt(alt) {
		}

		static Key FromKeyboardEvent(const KeyboardEvent& event) {
			return Key(event.GetKeyCode(), event.IsShiftPressed(), event.IsCtrlPressed(), event.IsAltPressed());
		}

		static Key ToNoShift(Key key) {
			return Key(key.keyCode, false, key.ctrl, key.alt);
		}

		constexpr bool operator==(const Key& other) const {
			return keyCode == other.keyCode && shift == other.shift && ctrl == other.ctrl && alt == other.alt;
		}

		constexpr bool operator!=(const Key& other) const {
			return !(*this == other);
		}
	};
}