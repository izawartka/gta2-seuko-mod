#include "wnd-proc.h"
#include "../hook-types/jump-hook.h"

static bool __stdcall DispatchWndProcEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	WndProcEvent event(hwnd, msg, wParam, lParam);
	Core::EventManager::GetInstance()->Dispatch(event);
	return event.IsHandled();
}

static DWORD wndProcHookReturnAddress = 0x004d0a08;

static __declspec(naked) void WndProcHookFunction(void)
{
	__asm {
		pushad
		push dword ptr [esp + 0x30]
		push dword ptr [esp + 0x30]
		push dword ptr [esp + 0x30]
		push dword ptr [esp + 0x30]
		call DispatchWndProcEvent
		test al, al
		jz L_eventNotHandled
		popad
		xor eax, eax
		ret

	L_eventNotHandled:
		popad
		sub esp, 0x10
		push ebx
		mov ebx, [esp + 0x20]
		jmp wndProcHookReturnAddress
	}
}

const JumpHook tryShootWeaponHook = {
	0x004d0a00,
	(DWORD)&WndProcHookFunction
};

bool WndProcEvent::Init()
{
	return Core::HookManager::GetInstance()->AddHook(tryShootWeaponHook);
}
