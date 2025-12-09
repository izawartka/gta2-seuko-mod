#include "keyboard.h"
#include "../hook-types/function-call-hook.h"

static bool isLeftShiftPressed = false;
static bool isLeftCtrlPressed = false;
static bool isLeftAltPressed = false;
static bool isRightShiftPressed = false;
static bool isRightCtrlPressed = false;
static bool isRightAltPressed = false;

static void UpdateModifierKeys(bool down, Game::KeyCode keyCode)
{
	switch (keyCode)
	{
	case Game::KeyCode::DIK_LSHIFT:
		isLeftShiftPressed = down;
		break;
	case Game::KeyCode::DIK_RSHIFT:
		isRightShiftPressed = down;
		break;
	case Game::KeyCode::DIK_LCONTROL:
		isLeftCtrlPressed = down;
		break;
	case Game::KeyCode::DIK_RCONTROL:
		isRightCtrlPressed = down;
		break;
	case Game::KeyCode::DIK_LMENU:
		isLeftAltPressed = down;
		break;
	case Game::KeyCode::DIK_RMENU:
		isRightAltPressed = down;
		break;
	default:
		break;
	}
}

static void DispatchKeyboardEvent()
{
	Game::KeyCode keyCode = *Game::Memory::GetKeyPressCode();
	int keyState = *Game::Memory::GetKeyPressState();
	bool isShiftPressed = isLeftShiftPressed || isRightShiftPressed;
	bool isCtrlPressed = isLeftCtrlPressed || isRightCtrlPressed;
	bool isAltPressed = isLeftAltPressed || isRightAltPressed;

	if (keyState == 0) {
		// Key up event
		UpdateModifierKeys(false, keyCode);
		KeyUpEvent event(keyCode, isShiftPressed, isCtrlPressed, isAltPressed);
		Core::EventManager::GetInstance()->Dispatch(event);
	}
	else {
		// Key down event
		UpdateModifierKeys(true, keyCode);
		KeyDownEvent event(keyCode, isShiftPressed, isCtrlPressed, isAltPressed);
		Core::EventManager::GetInstance()->Dispatch(event);
	}
}

static __declspec(naked) void OnKeyUpDownHookFunction(void)
{
	__asm {
		call DispatchKeyboardEvent
		jmp Game::Functions::OnKeyUpDown
	}
}

const FunctionCallHook keyboardHook = {
	0x0044c48d,
	(DWORD)&OnKeyUpDownHookFunction
};

bool KeyDownEvent::Init()
{
	return Core::HookManager::GetInstance()->AddHook(keyboardHook);
}

bool KeyUpEvent::Init()
{
	return Core::HookManager::GetInstance()->AddHook(keyboardHook);
}
