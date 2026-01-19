#include "game-start.h"
#include "../game/game.h"
#include "../hook-types/function-call-hook.h"

static void DispatchPreGameStartEvent()
{
	PreGameStartEvent event;
	Core::EventManager::GetInstance()->Dispatch(event);
}

static void DispatchGameStartEvent()
{
	GameStartEvent event;
	Core::EventManager::GetInstance()->Dispatch(event);
}

static __declspec(naked) void GameStartHookFunction(void)
{
	__asm {
		pushad
		call DispatchPreGameStartEvent
		popad
		call Game::Functions::InitGame
		pushad
		call DispatchGameStartEvent
		popad
		ret
	}
}

const FunctionCallHook gameStartHook = {
	0x004d14d2,
	(DWORD)&GameStartHookFunction
};

bool PreGameStartEvent::Init()
{
	return Core::HookManager::GetInstance()->AddHook(gameStartHook);
}

bool GameStartEvent::Init()
{
	return Core::HookManager::GetInstance()->AddHook(gameStartHook);
}
