#include "game-pause.h"
#include "../hook-types/jump-hook.h"

static bool __fastcall DispatchGamePauseEvent()
{
	GamePauseEvent event;
	Core::EventManager::GetInstance()->Dispatch(event);
	return event.GetDoCancelPause();
}

static __declspec(naked) void GamePauseHookFunction(void)
{
	__asm {
		pushad
		call DispatchGamePauseEvent
		test al, al
		popad
		jnz L_doCancelPause
		mov dword ptr [ecx], 0x2

	L_doCancelPause:
		ret
	}
}

const JumpHook gamePauseHook = {
	0x0045a6e6,
	(DWORD)&GamePauseHookFunction
};

bool GamePauseEvent::Init()
{
	return Core::HookManager::GetInstance()->AddHook(gamePauseHook);
}
