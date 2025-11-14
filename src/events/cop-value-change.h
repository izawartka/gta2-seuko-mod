#pragma once
#include "../core/core.h"
#include "../game/game.h"

/*
Dispatched when the game is about to change the player's cop value. (not all cases may be covered)
*/
class CopValueChangeEvent : public Core::EventBase {
public:
	static bool Init();
	CopValueChangeEvent(Game::ushort newValue) : m_newValue(newValue), m_modifiedNewValue(newValue) {};
	virtual ~CopValueChangeEvent() override {};

	Game::ushort GetNewValue() const { return m_newValue; }
	Game::ushort GetModifiedNewValue() const { return m_modifiedNewValue; }
	void SetModifiedNewValue(Game::ushort newValue) { m_modifiedNewValue = newValue; }
private:
	Game::ushort m_newValue;
	Game::ushort m_modifiedNewValue;
};

/*
Known not covered cases:
- When debug key '4' is pressed to add cop stars quickly (calls at 0x0043ab53 and 0x0043ab75)
*/
