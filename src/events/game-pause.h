#pragma once
#include "../core/core.h"

/*
Dispatched when the game being is paused.
*/
class GamePauseEvent : public Core::EventBase {
public:
	static bool Init();
	GamePauseEvent() {};
	virtual ~GamePauseEvent() override {};

	void SetDoCancelPause(bool doCancelPause) { m_doCancelPause = doCancelPause; }
	bool GetDoCancelPause() const { return m_doCancelPause; }

private:
	bool m_doCancelPause = false;
};
