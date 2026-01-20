#include "renderer-antialiasing-check.h"
#include "../hook-types/function-call-hook.h"

static bool __stdcall DispatchAntialiasingCheckEvent(uint32_t flags)
{
	RendererAntialiasingCheckEvent event(flags, (flags & 0x20000) == 0);
	Core::EventManager::GetInstance()->Dispatch(event);
	return !event.GetModifiedDoAntialiasing();
}

static __declspec(naked) void AntialiasingCheckHookFunction(void)
{
	__asm {
		inc [esp]
		pushad
		push edi
		call DispatchAntialiasingCheckEvent
		test al, al
		popad
		ret
	}
}

bool RendererAntialiasingCheckEvent::Init()
{
	if (!*Game::Memory::GetIsRendererLoaded()) return false;

	static const FunctionCallHook antialiasingHook = {
		Game::Memory::GetRendererBase() + 0x00e02e50,
		(DWORD)&AntialiasingCheckHookFunction
	};

	return Core::HookManager::GetInstance()->AddHook(antialiasingHook);
}
