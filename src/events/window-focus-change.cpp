#include "window-focus-change.h"
#include "../hook-types/jump-hook.h"

static void __fastcall DispatchWindowFocusChangeEvent(bool isFocused)
{
	WindowFocusChangeEvent event(isFocused);
	Core::EventManager::GetInstance()->Dispatch(event);
}

static DWORD WindowFocusChangeReturnAddress = 0x004d09d6;

static __declspec(naked) void WindowFocusChangeHookFunction(void)
{
	__asm {
		pushad
		call DispatchWindowFocusChangeEvent
		popad
		mov cl, byte ptr[0x00595018]
		jmp WindowFocusChangeReturnAddress
	}
}

const JumpHook windowFocusChangeHook = {
	0x004d09d0,
	(DWORD)&WindowFocusChangeHookFunction
};

bool WindowFocusChangeEvent::Init()
{
	return Core::HookManager::GetInstance()->AddHook(windowFocusChangeHook);
}
