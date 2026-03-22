#include "hide-cursor.h"
#include "../game/game.h"
#include "../hook-types/function-call-hook.h"

static bool __fastcall DispatchHideCursorEvent()
{
	HideCursorEvent event;
	Core::EventManager::GetInstance()->Dispatch(event);
	return event.GetDoCancelHide();
}

static __declspec(naked) void HideCursorHookFunction(void)
{
	__asm {
		pushad
		call DispatchHideCursorEvent
		test al, al
		popad
		jnz L_doCancelHide
		jmp Game::Functions::HideCursor

	L_doCancelHide:
		ret
	}
}

const FunctionCallHook hideCursorHook = {
	0x004cb639,
	(DWORD)&HideCursorHookFunction
};

bool HideCursorEvent::Init()
{
	return Core::HookManager::GetInstance()->AddHook(hideCursorHook);
}
