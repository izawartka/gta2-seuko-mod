#pragma once
#include "../core/core.h"

/*
Dispatched when the game being is unpaused.
*/
class GameUnpauseEvent : public Core::EventBase {
public:
	static bool Init();
	GameUnpauseEvent() {};
	virtual ~GameUnpauseEvent() override {};

	void SetDoCancelUnpause(bool doCancelUnpause) { m_doCancelUnpause = doCancelUnpause; }
	bool GetDoCancelUnpause() const { return m_doCancelUnpause; }

private:
	bool m_doCancelUnpause = false;
};
