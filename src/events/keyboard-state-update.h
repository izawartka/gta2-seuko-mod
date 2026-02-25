#pragma once
#include "../core/core.h"
#include "../game/game.h"

/*
Dispatched every frame, when the keyboard input has been parsed and the player's keyboard state is updated.
*/
class KeyboardStateUpdateEvent : public Core::EventBase {
public:
	static bool Init();
	KeyboardStateUpdateEvent(Game::Player* player, Game::KEYBOARD_STATE state) : m_player(player), m_state(state), m_modifiedState(state) {}
	virtual ~KeyboardStateUpdateEvent() override {};

	void SetModifiedState(Game::KEYBOARD_STATE modifiedState) { m_modifiedState = modifiedState; }
	Game::KEYBOARD_STATE GetState() const { return m_state; }
	Game::KEYBOARD_STATE GetModifiedState() const { return m_modifiedState; }
	Game::Player* GetPlayer() const { return m_player; }

private:
	Game::Player* m_player;
	Game::KEYBOARD_STATE m_state;
	Game::KEYBOARD_STATE m_modifiedState;
};
