#include "scale-shadow.h"
#include "../hook-types/function-call-hook.h"

static Game::SCR_f modifiedScale = 0;

static Game::SCR_f* __fastcall DispatchScaleShadowEvent(Game::SCR_f* scale)
{
	ScaleShadowEvent event(*scale);
	Core::EventManager::GetInstance()->Dispatch(event);
	modifiedScale = event.GetModifiedScale();
	return &modifiedScale;
}

static __declspec(naked) void ScaleShadowHookFunction(void)
{
	__asm {
		pushad
		call DispatchScaleShadowEvent
		mov [esp+0x18], eax
		popad
		jmp Game::Functions::ScaleScrf
	}
}

const FunctionCallHook scaleShadowHook = {
	0x004be394,
	(DWORD)&ScaleShadowHookFunction
};

bool ScaleShadowEvent::Init()
{
	return Core::HookManager::GetInstance()->AddHook(scaleShadowHook);
}
