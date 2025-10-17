#include "../core/core.h"
#include "../hook-types/jump-hook.h"
#include "after-debug-flags.h"

static void DispatchAfterDebugFlagsEvent()
{
	AfterDebugFlagsEvent event;
	Core::EventManager::GetInstance()->Dispatch(event);
}

static const DWORD GetSettingBooleanWithDefaultValueFn = 0x004b5ae0;

static __declspec(naked) void AfterDebugFlagsHookFunction(void)
{
	__asm {
		call GetSettingBooleanWithDefaultValueFn
		test eax, eax
		setnz al
		mov ds : [0x005ead76], al
		pop esi
		call DispatchAfterDebugFlagsEvent
		ret
	}
}

const JumpHook afterDebugFlagsHook = {
	0x00451f4f,
	(DWORD)&AfterDebugFlagsHookFunction
};

bool AfterDebugFlagsEvent::Init()
{
	return Core::HookManager::GetInstance()->AddHook(afterDebugFlagsHook);
}
