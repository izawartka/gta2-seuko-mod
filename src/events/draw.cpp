#include "../game/game.h"
#include "../core/core.h"
#include "../hook-types/function-call-hook.h"
#include "draw.h"

static void DispatchPreDrawUIEvent()
{
	PreDrawUIEvent event;
	Core::EventManager::GetInstance()->Dispatch(event);
}

static void DispatchDrawUIEvent()
{
	DrawUIEvent event;
	Core::EventManager::GetInstance()->Dispatch(event);
}

static __declspec(naked) void DrawUIHookFunction(void)
{
	__asm {
		push ecx
		call DispatchPreDrawUIEvent
		pop ecx
		call Game::Functions::DrawUI
		call DispatchDrawUIEvent
		ret
	}
}

const FunctionCallHook drawHook = {
	0x0045a61f,
	(DWORD)&DrawUIHookFunction
};

bool PreDrawUIEvent::Init()
{
	return Core::HookManager::GetInstance()->AddHook(drawHook);
}

bool DrawUIEvent::Init()
{
	return Core::HookManager::GetInstance()->AddHook(drawHook);
}
