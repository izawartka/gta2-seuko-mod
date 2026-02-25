#include "keyboard-state-update.h"
#include "../game/game.h"
#include "../hook-types/jump-hook.h"

static Game::KEYBOARD_STATE __stdcall DispatchKeyboardStateUpdateEvent(Game::Player* player, Game::KEYBOARD_STATE state)
{
	KeyboardStateUpdateEvent event(player, state);
	Core::EventManager::GetInstance()->Dispatch(event);
	return event.GetModifiedState();
}

static __declspec(naked) void SetKeyboardStateHookFunction(void)
{
	__asm {
		pushad
		push eax
		push ecx
		call DispatchKeyboardStateUpdateEvent
		mov dword ptr [esp + 0x1c], eax
		popad
		mov dword ptr [ecx + 0x4], eax
		ret 0x4
	}
}

const JumpHook setKeyboardStateHook = {
	0x004a4934,
	(DWORD)&SetKeyboardStateHookFunction
};

bool KeyboardStateUpdateEvent::Init()
{
	return Core::HookManager::GetInstance()->AddHook(setKeyboardStateHook);
}
