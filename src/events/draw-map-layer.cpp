#include "draw-map-layer.h"
#include "../hook-types/function-call-hook.h"

static bool __fastcall DispatchPreDrawMapLayerEvent(Game::S12* s12)
{
	PreDrawMapLayerEvent event(s12);
	Core::EventManager::GetInstance()->Dispatch(event);
	return event.GetSkipDraw();
}

static __declspec(naked) void DrawMapLayerHookFunction(void)
{
	__asm {
		pushad
		call DispatchPreDrawMapLayerEvent
		test eax, eax
		jz L_doDraw
		popad
		ret

	L_doDraw:
		popad
		jmp Game::Functions::DrawMapLayer
	}
}

const FunctionCallHook drawMapLayerHook = {
	0x00472535,
	(DWORD)&DrawMapLayerHookFunction
};

bool PreDrawMapLayerEvent::Init()
{
	return Core::HookManager::GetInstance()->AddHook(drawMapLayerHook);
}
