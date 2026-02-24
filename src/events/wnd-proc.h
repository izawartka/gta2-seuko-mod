#pragma once
#include "../core/core.h"

/*
Dispatched for every main window message received by the game.
Listeners can mark the event as handled to prevent the game and other listeners from processing the message.
Every listener should ignore an event if it is already marked as handled.
*/
class WndProcEvent : public Core::EventBase {
public:
	static bool Init();
	WndProcEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) : m_hwnd(hwnd), m_msg(msg), m_wParam(wParam), m_lParam(lParam) {}
	virtual ~WndProcEvent() override {};

	HWND GetHwnd() const { return m_hwnd; }
	UINT GetMsg() const { return m_msg; }
	WPARAM GetWParam() const { return m_wParam; }
	LPARAM GetLParam() const { return m_lParam; }
	bool IsHandled() const { return m_handled; }
	void SetHandled(bool handled) { m_handled = handled; }

private:
	HWND m_hwnd;
	UINT m_msg;
	WPARAM m_wParam;
	LPARAM m_lParam;
	bool m_handled = false;
};
