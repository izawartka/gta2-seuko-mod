#pragma once
#include "../core/core.h"

/*
Dispatched when the game tries to initialize mouse exclusive mode, when switching to fullscreen.
When the mouse is in exclusive mode, WM_MOUSEMOVE events are not being sent.
This event can be cancelled to prevent mouse exclusive mode from being initialized.
*/
class MouseExclusiveInitEvent : public Core::EventBase {
public:
	static bool Init();
	MouseExclusiveInitEvent() {};
	virtual ~MouseExclusiveInitEvent() override {};
	void SetDoCancelInit(bool doCancelInit) { m_doCancelInit = doCancelInit; }
	bool GetDoCancelInit() const { return m_doCancelInit; }

private:
	bool m_doCancelInit = false;
};
