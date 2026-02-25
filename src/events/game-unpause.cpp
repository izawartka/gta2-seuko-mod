#include "game-unpause.h"
#include "../game/game.h"
#include "../hook-types/jump-hook.h"

static bool __fastcall DispatchGameUnpauseEvent()
{
	GameUnpauseEvent event;
	Core::EventManager::GetInstance()->Dispatch(event);
	return event.GetDoCancelUnpause();
}

static __declspec(naked) void GameUnpauseHookFunction(void)
{
	__asm {
		pushad
		call DispatchGameUnpauseEvent
		test al, al
		popad
		jnz L_doCancelUnpause
		jmp Game::Functions::UnpauseGame

	L_doCancelUnpause:
		ret
	}
}

const JumpHook gameUnpauseHook = {
	0x0045baaa,
	(DWORD)&GameUnpauseHookFunction
};

bool GameUnpauseEvent::Init()
{
	return Core::HookManager::GetInstance()->AddHook(gameUnpauseHook);
}
