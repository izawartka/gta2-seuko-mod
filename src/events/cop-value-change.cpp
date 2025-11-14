#include "cop-value-change.h"
#include "../hook-types/jump-hook.h"
#include "../hook-types/function-multi-call-hook.h"
#include "../hook-types/function-call-hook.h"

static Game::ushort __stdcall DispatchCopValueChangeEvent(Game::ushort newValue)
{
	CopValueChangeEvent event(newValue);
	Core::EventManager::GetInstance()->Dispatch(event);
	return event.GetModifiedNewValue();
}

static const DWORD CopKillHookReturnAddress = 0x0043756c;

static __declspec(naked) void CopKillHookFunction(void)
{
	__asm {
		push esi
		push ebx
		call DispatchCopValueChangeEvent
		pop esi
		mov WORD ptr[esi + 0x20a], ax
		jmp CopKillHookReturnAddress
	}
}

const JumpHook copKillHook = {
	0x00437565,
	(DWORD)&CopKillHookFunction
};

static const DWORD AddCopValueHook1ReturnAddress = 0x00435402;

static __declspec(naked) void AddCopValueHook1Function(void)
{
	__asm {
		push ecx
		push 0
		call DispatchCopValueChangeEvent
		pop ecx
		mov WORD ptr[ecx + 0x20a], ax
		jmp AddCopValueHook1ReturnAddress
	}
}

const JumpHook addCopValueHook1 = {
	0x004353f9,
	(DWORD)&AddCopValueHook1Function
};

static const DWORD AddCopValueHook2ReturnAddress = 0x004353f2;

static __declspec(naked) void AddCopValueHook2Function(void)
{
	__asm {
		push ecx
		push 0x2ee0
		call DispatchCopValueChangeEvent
		pop ecx
		mov WORD ptr[ecx + 0x20a], ax
		jmp AddCopValueHook2ReturnAddress
	}
}

const JumpHook addCopValueHook2 = {
	0x004353e9,
	(DWORD)&AddCopValueHook2Function
};

static const DWORD AddCopValueHook3ReturnAddress = 0x004353dc;

static __declspec(naked) void AddCopValueHook3Function(void)
{
	__asm {
		push ecx
		push eax
		call DispatchCopValueChangeEvent
		pop ecx
		add WORD ptr[ecx + 0x20a], ax
		jmp AddCopValueHook3ReturnAddress
	}
}

const JumpHook addCopValueHook3 = {
	0x004353d5,
	(DWORD)&AddCopValueHook3Function
};

static const DWORD ExplodeCarHookReturnAddress = 0x0042715a;

static __declspec(naked) void ExplodeCarHookFunction(void)
{
	__asm {
		push edi
		push eax
		call DispatchCopValueChangeEvent
		pop edi
		mov WORD ptr[edi + 0x20a], ax
		jmp ExplodeCarHookReturnAddress
	}
}

const JumpHook explodeCarHook = {
	0x00427153,
	(DWORD)&ExplodeCarHookFunction
};

static __declspec(naked) void SetCopValue0HookFunction(void)
{
	__asm {
		push ecx
		push 0
		call DispatchCopValueChangeEvent
		pop ecx
		mov WORD ptr[ecx + 0x20a], ax
		ret
	}
}

const JumpHook setCopValue0Hook = {
	0x00420b80,
	(DWORD)&SetCopValue0HookFunction
};

static const DWORD SetCopStarsOriginalFn = 0x00434cd0;

static __declspec(naked) void SetCopStarsHookFunction()
{
	__asm {
		push edx
		push ecx
		mov eax, DWORD ptr[esp + 0x4]
		push eax
		call DispatchCopValueChangeEvent
		mov DWORD ptr[esp + 0x4], eax
		pop ecx
		pop edx
		jmp SetCopStarsOriginalFn
	}
}

const FunctionMultiCallHook setCopStarsHook = {
	{0x00435422, 0x0047b7f4},
	(DWORD)&SetCopStarsHookFunction
};

bool CopValueChangeEvent::Init()
{
	return Core::HookManager::GetInstance()->AddHook(copKillHook) &&
		Core::HookManager::GetInstance()->AddHook(addCopValueHook1) &&
		Core::HookManager::GetInstance()->AddHook(addCopValueHook2) &&
		Core::HookManager::GetInstance()->AddHook(addCopValueHook3) &&
		Core::HookManager::GetInstance()->AddHook(explodeCarHook) &&
		Core::HookManager::GetInstance()->AddHook(setCopValue0Hook) &&
		Core::HookManager::GetInstance()->AddHook(setCopStarsHook);
}
