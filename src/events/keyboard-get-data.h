#pragma once
#include "../core/core.h"
#include "../game/game.h"

/*
Dispatched when the game is getting keyboard data and can be used to emulate keyboard input. 
One keyboard input  per tick can be registered by the game, so check IsReadyToEmulate() before emulating a key.
*/
class KeyboardGetDataEvent : public Core::EventBase {
public:
	static bool Init();
	KeyboardGetDataEvent(DWORD dataCount, Game::KeyCode keyCode, bool isDown)
		: m_dataCount(dataCount), m_keyCode(keyCode), m_isDown(isDown), m_modifiedDataCount(dataCount), m_modifiedKeyCode(keyCode), m_modifiedIsDown(isDown) { }
	virtual ~KeyboardGetDataEvent() override {};

	DWORD GetDataCount() const { return m_dataCount; }
	DWORD GetModifiedDataCount() const { return m_modifiedDataCount; }
	Game::KeyCode GetKeyCode() const { return m_keyCode; }
	Game::KeyCode GetModifiedKeyCode() const { return m_modifiedKeyCode; }
	bool IsDown() const { return m_isDown; }
	bool GetModifiedIsDown() const { return m_modifiedIsDown; }

	bool IsReadyToEmulate() const { return m_modifiedDataCount == 0; }
	void EmulateKey(Game::KeyCode keyCode, bool isDown) {
		m_modifiedDataCount = 1;
		m_modifiedKeyCode = keyCode;
		m_modifiedIsDown = isDown;
	}
	void Drop() {
		m_modifiedDataCount = 0;
		m_modifiedKeyCode = Game::KeyCode::DIK_NONE;
		m_modifiedIsDown = false;
	}

private:
	DWORD m_dataCount;
	DWORD m_modifiedDataCount;
	Game::KeyCode m_keyCode;
	Game::KeyCode m_modifiedKeyCode;
	bool m_isDown;
	bool m_modifiedIsDown;
};
