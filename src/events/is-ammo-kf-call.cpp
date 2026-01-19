#include "is-ammo-kf-call.h"
#include "../hook-types/function-multi-call-hook.h"

static bool __fastcall DispatchIsAmmoKfCallEvent(bool value)
{
	IsAmmoKfCallEvent event(value);
	Core::EventManager::GetInstance()->Dispatch(event);
	return event.GetModifiedValue();
}

static __declspec(naked) void IsAmmoKfCallHookFunction(void)
{
	__asm {
		xor eax, eax
		cmp WORD PTR [ecx], 0xffff
		sete al
		mov ecx, eax
		pushad
		call DispatchIsAmmoKfCallEvent
		mov [esp + 0x1c], eax
		popad
		ret
	}
}

const FunctionMultiCallHook isAmmoKfCallHook = {
	{
		0x004cca3c,
		0x004cca63,
	},
	(DWORD)&IsAmmoKfCallHookFunction
};

bool IsAmmoKfCallEvent::Init()
{
	return Core::HookManager::GetInstance()->AddHook(isAmmoKfCallHook);
}
