#pragma once
#include "../core/core.h"

/*
Dispatched when the window focus changes.
GetIsFocused() returns the new focus state of the window.
*/
class WindowFocusChangeEvent : public Core::EventBase {
public:
	static bool Init();
	WindowFocusChangeEvent(bool isFocused) : m_isFocused(isFocused) {}
	virtual ~WindowFocusChangeEvent() override {};

	bool GetIsFocused() const { return m_isFocused; }
	
private:
	bool m_isFocused;
};
