#pragma once
#include "../core/core.h"
#include "../game/game.h"

/*
Dispatched when a key is pressed down.
*/
class KeyDownEvent : public Core::EventBase {
public:
	static bool Init();
	KeyDownEvent(Game::KeyCode keyCode, bool shift = false, bool ctrl = false, bool alt = false) 
		: m_keyCode(keyCode), m_shift(shift), m_ctrl(ctrl), m_alt(alt) {
	};
	virtual ~KeyDownEvent() override {};

	Game::KeyCode GetKeyCode() const { return m_keyCode; }
	bool IsShiftPressed() const { return m_shift; }
	bool IsCtrlPressed() const { return m_ctrl; }
	bool IsAltPressed() const { return m_alt; }

private:
	Game::KeyCode m_keyCode;
	bool m_shift = false;
	bool m_ctrl = false;
	bool m_alt = false;
};

/*
Dispatched when a key is released.
*/
class KeyUpEvent : public Core::EventBase {
public:
	static bool Init();
	KeyUpEvent(Game::KeyCode keyCode, bool shift = false, bool ctrl = false, bool alt = false) 
		: m_keyCode(keyCode), m_shift(shift), m_ctrl(ctrl), m_alt(alt) {
	};
	virtual ~KeyUpEvent() override {};

	Game::KeyCode GetKeyCode() const { return m_keyCode; }
	bool IsShiftPressed() const { return m_shift; }
	bool IsCtrlPressed() const { return m_ctrl; }
	bool IsAltPressed() const { return m_alt; }

private:
	Game::KeyCode m_keyCode;
	bool m_shift = false;
	bool m_ctrl = false;
	bool m_alt = false;
};
