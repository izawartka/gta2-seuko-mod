#pragma once
#include "../core/core.h"

/*
Dispatched right before every frame is drawn.
Things drawn here will probably stay covered.

Note: Game::Memory::GetMainCamera() may be invalid when this event is dispatched on first frame of a level. Use game->currentPlayer->ph2 instead.
*/
class PreDrawFrameEvent : public Core::EventBase {
public:
	static bool Init();
	PreDrawFrameEvent() {};
	virtual ~PreDrawFrameEvent() override {};
};

/*
Dispatched right after the each frame is drawn, just before flipping the buffers.
Things drawn here will appear on top of everything.
*/
class DrawFrameEvent : public Core::EventBase {
public:
	static bool Init();
	DrawFrameEvent() {};
	virtual ~DrawFrameEvent() override {};
};

/*
Dispatched when the game is about to clear after flipping the buffers.
Note: DoClear is false unless some skip tile cheats are on (this is the default game behavior)
*/
class ClearScreenEvent : public Core::EventBase {
public:
	static bool Init();
	ClearScreenEvent(bool doClear) : m_doClear(doClear) {};
	virtual ~ClearScreenEvent() override {};

	void SetDoClear(bool doClear) { m_doClear = doClear; }
	bool GetDoClear() const { return m_doClear;  }

private:
	bool m_doClear;
};
