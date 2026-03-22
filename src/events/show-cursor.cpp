#include "show-cursor.h"
#include "../game/game.h"
#include "../hook-types/function-call-hook.h"

static bool __fastcall DispatchShowCursorEvent()
{
	ShowCursorEvent event;
	Core::EventManager::GetInstance()->Dispatch(event);
	return event.GetDoCancelShow();
}

static __declspec(naked) void ShowCursorHookFunction(void)
{
	__asm {
		pushad
		call DispatchShowCursorEvent
		test al, al
		popad
		jnz L_doCancelShow
		jmp Game::Functions::ShowCursor

	L_doCancelShow:
		ret
	}
}

const FunctionCallHook showCursorHook = {
	0x004cb863,
	(DWORD)&ShowCursorHookFunction
};

bool ShowCursorEvent::Init()
{
	return Core::HookManager::GetInstance()->AddHook(showCursorHook);
}
