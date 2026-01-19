#include "game-tick.h"
#include "../game/game.h"
#include "../hook-types/function-multi-call-hook.h"

static void DispatchPreGameTickEvent()
{
	PreGameTickEvent event;
	Core::EventManager::GetInstance()->Dispatch(event);
}

static void DispatchGameTickEvent()
{
	GameTickEvent event;
	Core::EventManager::GetInstance()->Dispatch(event);
}

static __declspec(naked) void GameTickHookFunction(void)
{
	__asm {
		pushad
		call DispatchPreGameTickEvent
		popad
		call Game::Functions::GameTick
		pushad
		call DispatchGameTickEvent
		popad
		ret
	}
}

const FunctionMultiCallHook gameTickHook = {
	std::vector<DWORD>{ 0x0045c3f2, 0x0045c3e9 },
	(DWORD)&GameTickHookFunction
};

bool PreGameTickEvent::Init()
{
	return Core::HookManager::GetInstance()->AddHook(gameTickHook);
}

bool GameTickEvent::Init()
{
	return Core::HookManager::GetInstance()->AddHook(gameTickHook);
}
