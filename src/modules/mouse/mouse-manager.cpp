#include "mouse-manager.h"
#include "windowsx.h"
#include "events/mouse.h"
#include "../../game/game.h"

MouseModule::MouseManager* MouseModule::MouseManager::m_instance = nullptr;

MouseModule::MouseManager* MouseModule::MouseManager::GetInstance() {
	assert(m_instance != nullptr && "MouseManager not initialized!");
	return m_instance;
}

bool MouseModule::MouseManager::IsSafeToInitEvents() {
	return m_instance && m_instance->m_attached;
}

MouseModule::MouseState MouseModule::MouseManager::FetchMouseState()
{
	MouseState state;
	HWND hwnd = *Game::Memory::GetGameWindow();

	POINT p;
	GetCursorPos(&p);
	HWND hovered = WindowFromPoint(p);
	state.isInWindow = (GetAncestor(hovered, GA_ROOT) == hwnd);

	ScreenToClient(hwnd, &p);
	state.position = { p.x, p.y };

	RECT clientRect;
	GetClientRect(hwnd, &clientRect);

	if (state.position.x < 0) state.position.x = 0;
	if (state.position.y < 0) state.position.y = 0;
	if (state.position.x >= clientRect.right) state.position.x = clientRect.right - 1;
	if (state.position.y >= clientRect.bottom) state.position.y = clientRect.bottom - 1;

	state.leftButtonDown = state.isInWindow && (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
	state.rightButtonDown = state.isInWindow && (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;
	state.middleButtonDown = state.isInWindow && (GetAsyncKeyState(VK_MBUTTON) & 0x8000) != 0;

	return state;
}

MouseModule::NormalizedMousePosition MouseModule::MouseManager::ToNormalizedPosition(const MousePosition& pos)
{
	MousePosition clientSize = GetClientAreaSize();
	if (clientSize.x == 0 || clientSize.y == 0) return { 0.0f, 0.0f };
	return {
		static_cast<float>(pos.x) / clientSize.x,
		static_cast<float>(pos.y) / clientSize.y
	};
}

MouseModule::MousePosition MouseModule::MouseManager::ToPixelPosition(const NormalizedMousePosition& normalizedPos)
{
	MousePosition clientSize = GetClientAreaSize();
	return {
		static_cast<int>(normalizedPos.x * clientSize.x),
		static_cast<int>(normalizedPos.y * clientSize.y)
	};
}

bool MouseModule::MouseManager::MarkEventInitialized()
{
	if(!m_attached) {
		spdlog::error("Cannot initialize MouseManager events: MouseManager is not attached");
		return false;
	}

	m_initializedEvents++;
	if (m_initializedEvents == 1) {
		UpdateWndProcEventListener();
		UpdateState();
	}

	return true;
}

void MouseModule::MouseManager::MarkEventDeinitialized()
{
	m_initializedEvents--;
	if (m_initializedEvents <= 0) {
		m_initializedEvents = 0;
		UpdateWndProcEventListener();
	}
}

std::optional<MouseModule::MouseState> MouseModule::MouseManager::GetLastMouseState() const
{
	if (!m_attached) return std::nullopt;
	if (m_initializedEvents == 0) std::nullopt;

	return m_state;
}

void MouseModule::MouseManager::SetLocked(bool locked)
{
	m_locked = locked;
	if(locked) UpdateCursorOwned();
	UpdateLockedState();
}

void MouseModule::MouseManager::SetInvisible(bool invisible)
{
	m_invisible = invisible;
	if (invisible) UpdateCursorOwned();
	UpdateInvisible();
}

MouseModule::MouseManager::MouseManager() {
	assert(!m_instance && "MouseManager instance already exists");
	m_instance = this;
}

MouseModule::MouseManager::~MouseManager() {
	m_instance = nullptr;
}

void MouseModule::MouseManager::SetToCenter()
{
	POINT previousPos;
	GetCursorPos(&previousPos);

	HWND hwnd = *Game::Memory::GetGameWindow();
	RECT rect;
	GetClientRect(hwnd, &rect);

	POINT center;
	center.x = (rect.right - rect.left) / 2;
	center.y = (rect.bottom - rect.top) / 2;

	ClientToScreen(hwnd, &center);

	if (previousPos.x != center.x || previousPos.y != center.y) {
		SetCursorPos(center.x, center.y);
	}
}

MouseModule::MousePosition MouseModule::MouseManager::GetClientAreaSize()
{
	HWND hwnd = *Game::Memory::GetGameWindow();
	RECT rect;
	GetClientRect(hwnd, &rect);
	return MousePosition{ rect.right - 1, rect.bottom - 1 };
}

void MouseModule::MouseManager::OnWndProcEvent(WndProcEvent& event)
{
	if (event.IsHandled()) return;
	Core::EventManager* eventManager = Core::EventManager::GetInstance();

	switch (event.GetMsg()) {
	case WM_MOUSEMOVE: {
		if (!m_tracking) SetupTracking();
		if (!m_state.isInWindow) OnMouseEnter();

		MousePosition currentPosition = {
			GET_X_LPARAM(event.GetLParam()),
			GET_Y_LPARAM(event.GetLParam())
		};

		MousePosition previousPosition = m_state.position;
		m_state.position = currentPosition;

		MouseMoveEvent mouseMoveEvent(currentPosition);
		eventManager->Dispatch(mouseMoveEvent);

		return;
	}
	case WM_LBUTTONDOWN: {
		m_state.leftButtonDown = true;

		MouseButtonDownEvent leftButtonDownEvent(MouseButton::Left);
		eventManager->Dispatch(leftButtonDownEvent);
		break;
	}
	case WM_LBUTTONUP: {
		m_state.leftButtonDown = false;

		MouseButtonUpEvent leftButtonUpEvent(MouseButton::Left);
		eventManager->Dispatch(leftButtonUpEvent);
		break;
	}
	case WM_RBUTTONDOWN: {
		m_state.rightButtonDown = true;

		MouseButtonDownEvent rightButtonDownEvent(MouseButton::Right);
		eventManager->Dispatch(rightButtonDownEvent);
		break;
	}
	case WM_RBUTTONUP: {
		m_state.rightButtonDown = false;

		MouseButtonUpEvent rightButtonUpEvent(MouseButton::Right);
		eventManager->Dispatch(rightButtonUpEvent);
		break;
	}
	case WM_MBUTTONDOWN: {
		m_state.middleButtonDown = true;

		MouseButtonDownEvent middleButtonDownEvent(MouseButton::Middle);
		eventManager->Dispatch(middleButtonDownEvent);
		break;
	}
	case WM_MBUTTONUP: {
		m_state.middleButtonDown = false;

		MouseButtonUpEvent middleButtonUpEvent(MouseButton::Middle);
		eventManager->Dispatch(middleButtonUpEvent);
		break;
	}
	case WM_MOUSELEAVE: {
		m_state.isInWindow = false;
		m_tracking = false;

		MouseLeaveEvent mouseLeaveEvent;
		eventManager->Dispatch(mouseLeaveEvent);

		ResetButtons();
		break;
	}
	case WM_SETCURSOR: {
		if (m_invisible && m_cursorOwned) {
			SetCursor(NULL);
			event.SetHandled(true);
		}
		break;
	}
	case WM_KILLFOCUS:
	case WM_SETFOCUS:
	case WM_SIZE:
	case WM_ACTIVATE:
		UpdateCursorOwned();
		UpdateLockedState();
		UpdateInvisible();
		break;
	};
}

void MouseModule::MouseManager::OnPreDrawFrame(PreDrawFrameEvent& event)
{
	if (!m_locked || !m_cursorOwned) return;

	MousePosition movePosition = m_state.position;

	POINT centerPoint;
	HWND hwnd = *Game::Memory::GetGameWindow();
	RECT rect;
	GetClientRect(hwnd, &rect);
	centerPoint.x = rect.right / 2;
	centerPoint.y = rect.bottom / 2;

	MousePosition delta = {
		movePosition.x - centerPoint.x,
		movePosition.y - centerPoint.y
	};

	if (delta.x != 0 || delta.y != 0) {
		MouseLockedMoveEvent mouseLockedMoveEvent(delta);
		Core::EventManager* eventManager = Core::EventManager::GetInstance();
		eventManager->Dispatch(mouseLockedMoveEvent);
	}

	SetToCenter();
}

bool MouseModule::MouseManager::Attach()
{
	m_attached = true;
	return true;
}

void MouseModule::MouseManager::Detach()
{
	m_attached = false;
	SetLocked(false);
	SetInvisible(false);
	if (HasEventListener<WndProcEvent>()) {
		spdlog::error("MouseManager is being detached while having uninitialized events");
		m_initializedEvents = 0;
		RemoveEventListener<WndProcEvent>();
	}
}

void MouseModule::MouseManager::UpdateState()
{
	m_state = FetchMouseState();
}

void MouseModule::MouseManager::SetupTracking()
{
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.dwFlags = TME_LEAVE;
	tme.hwndTrack = *Game::Memory::GetGameWindow();
	TrackMouseEvent(&tme);

	m_tracking = true;
}

void MouseModule::MouseManager::OnMouseEnter()
{
	m_state.isInWindow = true;

	Core::EventManager* eventManager = Core::EventManager::GetInstance();
	MouseEnterEvent mouseEnterEvent;
	eventManager->Dispatch(mouseEnterEvent);

	if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0) {
		m_state.leftButtonDown = true;
		MouseButtonDownEvent leftButtonDownEvent(MouseButton::Left);
		eventManager->Dispatch(leftButtonDownEvent);
	}

	if ((GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0) {
		m_state.rightButtonDown = true;
		MouseButtonDownEvent rightButtonDownEvent(MouseButton::Right);
		eventManager->Dispatch(rightButtonDownEvent);
	}

	if ((GetAsyncKeyState(VK_MBUTTON) & 0x8000) != 0) {
		m_state.middleButtonDown = true;
		MouseButtonDownEvent middleButtonDownEvent(MouseButton::Middle);
		eventManager->Dispatch(middleButtonDownEvent);
	}
}

void MouseModule::MouseManager::ResetButtons()
{
	Core::EventManager* eventManager = Core::EventManager::GetInstance();

	if(m_state.leftButtonDown) {
		m_state.leftButtonDown = false;
		MouseButtonUpEvent leftButtonUpEvent(MouseButton::Left);
		eventManager->Dispatch(leftButtonUpEvent);
	}

	if (m_state.rightButtonDown) {
		m_state.rightButtonDown = false;
		MouseButtonUpEvent rightButtonUpEvent(MouseButton::Right);
		eventManager->Dispatch(rightButtonUpEvent);
	}

	if (m_state.middleButtonDown) {
		m_state.middleButtonDown = false;
		MouseButtonUpEvent middleButtonUpEvent(MouseButton::Middle);
		eventManager->Dispatch(middleButtonUpEvent);
	}
}

void MouseModule::MouseManager::UpdateWndProcEventListener()
{
	bool shouldListen = (m_initializedEvents > 0) || m_locked || m_invisible;
	SetEventListener<WndProcEvent>(&MouseManager::OnWndProcEvent, shouldListen);
	if (!shouldListen) m_tracking = false;
}

void MouseModule::MouseManager::UpdateCursorOwned()
{
	HWND hwnd = *Game::Memory::GetGameWindow();
	HWND foreground = GetForegroundWindow();
	bool isFocused = (GetForegroundWindow() == hwnd);
	bool isMinimized = IsIconic(hwnd);

	m_cursorOwned = isFocused && !isMinimized;
}

void MouseModule::MouseManager::UpdateLockedState()
{
	bool locked = m_locked && m_cursorOwned;

	UpdateWndProcEventListener();
	SetEventListener<PreDrawFrameEvent>(&MouseManager::OnPreDrawFrame, locked);
	if (locked) SetToCenter();
}

void MouseModule::MouseManager::UpdateInvisible()
{
	UpdateWndProcEventListener();

	if (m_invisible && m_cursorOwned) {
		SetCursor(NULL);
	}
	else {
		HCURSOR defaultCursor = LoadCursor(NULL, IDC_ARROW);
		SetCursor(defaultCursor);
	}
}
