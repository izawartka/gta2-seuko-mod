#pragma once
#include "../core/core.h"

/*
Dispatched when the game tries to show the cursor. This event can be cancelled.
Note: a call at 0x004cb8ae won't fire the event, because it is related to SVideo cleanup.
*/
class ShowCursorEvent : public Core::EventBase {
public:
	static bool Init();
	ShowCursorEvent() {};
	virtual ~ShowCursorEvent() override {};

	void SetDoCancelShow(bool doCancelShow) { m_doCancelShow = doCancelShow; }
	bool GetDoCancelShow() const { return m_doCancelShow; }

private:
	bool m_doCancelShow = false;
};
