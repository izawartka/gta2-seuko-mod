#pragma once
#include "../core/core.h"

/*
Dispatched when the game tries to hide the cursor. This event can be cancelled.
*/
class HideCursorEvent : public Core::EventBase {
public:
	static bool Init();
	HideCursorEvent() {};
	virtual ~HideCursorEvent() override {};

	void SetDoCancelHide(bool doCancelHide) { m_doCancelHide = doCancelHide; }
	bool GetDoCancelHide() const { return m_doCancelHide; }

private:
	bool m_doCancelHide = false;
};
