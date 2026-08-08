#pragma once
#include "../common.h"

namespace KeyboardModule {
	/*
	Base class for all keyboard events. Not to be used directly.
	*/
	class KeyboardEventBase : public Core::CancellableEventBase {
	public:
		static bool Init() { return false; }
		Game::KeyCode GetKeyCode() const { return m_keyCode; }
		bool IsShiftPressed() const { return m_shift; }
		bool IsCtrlPressed() const { return m_ctrl; }
		bool IsAltPressed() const { return m_alt; }
		bool IsCapsLockOn() const { return m_capsLock; }

	protected:
		KeyboardEventBase(Game::KeyCode keyCode, bool shift = false, bool ctrl = false, bool alt = false, bool capsLock = false)
			: m_keyCode(keyCode), m_shift(shift), m_ctrl(ctrl), m_alt(alt), m_capsLock(capsLock) {}
		virtual ~KeyboardEventBase() override {};

		Game::KeyCode m_keyCode;
		bool m_shift = false;
		bool m_ctrl = false;
		bool m_alt = false;
		bool m_capsLock = false;
	};
}
