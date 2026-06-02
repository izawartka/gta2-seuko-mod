#pragma once
#include "../core/core.h"
#include "../game/game.h"

/*
Dispatched when the game is about to calculate screen position for a player name label.
Absolute world position can be read and modified.
*/
class DrawPlayerNameEvent : public Core::EventBase {
public:
	static bool Init();
	DrawPlayerNameEvent(const Game::SCR_Vector3& position) :
		m_position(position),
		m_modifiedPosition(position) {};
	virtual ~DrawPlayerNameEvent() override {};

	const Game::SCR_Vector3& GetPosition() const { return m_position; }
	void SetModifiedPosition(const Game::SCR_Vector3& modifiedPosition) { m_modifiedPosition = modifiedPosition; }
	const Game::SCR_Vector3& GetModifiedPosition() const { return m_modifiedPosition; }

private:
	Game::SCR_Vector3 m_position;
	Game::SCR_Vector3 m_modifiedPosition;
};
