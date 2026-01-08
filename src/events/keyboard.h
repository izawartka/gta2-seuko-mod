#pragma once
#include "../core/core.h"
#include "../game/game.h"

/*
Base class for all keyboard events. Not to be used directly.
*/
class KeyboardEvent : public Core::EventBase {
public:
	Game::KeyCode GetKeyCode() const { return m_keyCode; }
	bool IsShiftPressed() const { return m_shift; }
	bool IsCtrlPressed() const { return m_ctrl; }
	bool IsAltPressed() const { return m_alt; }

protected:
	KeyboardEvent(Game::KeyCode keyCode, bool shift = false, bool ctrl = false, bool alt = false)
		: m_keyCode(keyCode), m_shift(shift), m_ctrl(ctrl), m_alt(alt) {
	};
	virtual ~KeyboardEvent() override {};

	Game::KeyCode m_keyCode;
	bool m_shift = false;
	bool m_ctrl = false;
	bool m_alt = false;
};

/*
Dispatched when a key is pressed down.
*/
class KeyDownEvent : public KeyboardEvent {
public:
	static bool Init();
	KeyDownEvent(Game::KeyCode keyCode, bool shift = false, bool ctrl = false, bool alt = false) 
		: KeyboardEvent(keyCode, shift, ctrl, alt) { }
};

/*
Dispatched when a key is released.
*/
class KeyUpEvent : public KeyboardEvent {
public:
	static bool Init();
	KeyUpEvent(Game::KeyCode keyCode, bool shift = false, bool ctrl = false, bool alt = false)
		: KeyboardEvent(keyCode, shift, ctrl, alt) { }
};
