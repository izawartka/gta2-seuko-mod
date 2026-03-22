#include "mouse-exclusive-init.h"
#include "../game/game.h"
#include "../hook-types/function-call-hook.h"

static bool __fastcall DispatchMouseExclusiveInitEvent()
{
	MouseExclusiveInitEvent event;
	Core::EventManager::GetInstance()->Dispatch(event);
	return event.GetDoCancelInit();
}

static __declspec(naked) void MouseExclusiveInitHookFunction(void)
{
	__asm {
		pushad
		call DispatchMouseExclusiveInitEvent
		test al, al
		popad
		jnz L_doCancelInit
		jmp Game::Functions::InitMouseExclusive

	L_doCancelInit:
		ret
	}
}

const FunctionCallHook mouseExclusiveInitHook = {
	0x004cb689,
	(DWORD)&MouseExclusiveInitHookFunction
};

bool MouseExclusiveInitEvent::Init()
{
	return Core::HookManager::GetInstance()->AddHook(mouseExclusiveInitHook);
}
