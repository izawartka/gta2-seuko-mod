#include "../game/game.h"
#include "../core/core.h"
#include "../hook-types/function-call-hook.h"
#include "keyboard.h"

static int isShiftPressed = false;
static int isCtrlPressed = false;
static int isAltPressed = false;

static void UpdateModifierKeys(bool down, Game::KeyCode keyCode)
{
	if(keyCode == Game::KeyCode::DIK_LSHIFT) {
		isShiftPressed = down;
	}
	else if (keyCode == Game::KeyCode::DIK_LCONTROL) {
		isCtrlPressed = down;
	}
	else if (keyCode == Game::KeyCode::DIK_LMENU) {
		isAltPressed = down;
	}
}

static void DispatchKeyboardEvent()
{
	Game::KeyCode keyCode = *Game::Memory::GetKeyPressCode();
	int keyState = *Game::Memory::GetKeyPressState();

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
