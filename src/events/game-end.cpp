#include "game-end.h"
#include "../game/game.h"
#include "../hook-types/function-multi-call-hook.h"

static void DispatchPreGameEndEvent()
{
	PreGameEndEvent event;
	Core::EventManager::GetInstance()->Dispatch(event);
}

static void DispatchGameEndEvent()
{
	GameEndEvent event;
	Core::EventManager::GetInstance()->Dispatch(event);
}

static __declspec(naked) void GameEndHookFunction(void)
{
	__asm {
		pushad
		call DispatchPreGameEndEvent
		popad
		call Game::Functions::DeinitGame
		pushad
		call DispatchGameEndEvent
		popad
		ret
	}
}

const FunctionMultiCallHook gameEndHook = {
	std::vector<DWORD>{
		0x004620ba,
		0x004d1606,
		0x004d169a,
		0x004d16ee,
		0x004d1742,
		0x004d1755
	},
	(DWORD)&GameEndHookFunction
};

bool PreGameEndEvent::Init()
{
	return Core::HookManager::GetInstance()->AddHook(gameEndHook);
}

bool GameEndEvent::Init()
{
	return Core::HookManager::GetInstance()->AddHook(gameEndHook);
}
