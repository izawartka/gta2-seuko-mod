#include "cop-value-change.h"
#include "../hook-types/jump-hook.h"

static Game::ushort __stdcall DispatchCopValueChangeEvent(Game::Ped* ped, Game::ushort newValue)
{
	newValue &= 0xffff;
	CopValueChangeEvent event(ped, newValue);
	Core::EventManager::GetInstance()->Dispatch(event);
	return event.GetModifiedNewValue();
}

static Game::ushort __stdcall DispatchCopValueChangeEventWithStars(Game::Ped* ped, Game::uchar newStarValue)
{
	newStarValue &= 0xff;
	Game::ushort newValue = Game::Utils::StarsToCopValue(newStarValue);
	CopValueChangeEvent event(ped, newValue);
	Core::EventManager::GetInstance()->Dispatch(event);
	return event.GetModifiedNewValue();
}

static const DWORD CopKillHookReturnAddress = 0x0043756c;

static __declspec(naked) void CopKillHookFunction(void)
{
	__asm {
		pushad
		push ebx
		push esi
		call DispatchCopValueChangeEvent
		mov esi, dword ptr[esp + 0x4]
		mov word ptr[esi + 0x20a], ax
		popad
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
		pushad
		push 0
		push ecx
		call DispatchCopValueChangeEvent
		mov ecx, dword ptr[esp + 0x18]
		mov word ptr[ecx + 0x20a], ax
		popad
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
		pushad
		push 0x2ee0
		push ecx
		call DispatchCopValueChangeEvent
		mov ecx, dword ptr[esp + 0x18]
		mov word ptr[ecx + 0x20a], ax
		popad
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
		pushad
		add ax, word ptr[ecx + 0x20a]
		push eax
		push ecx
		call DispatchCopValueChangeEvent
		mov ecx, dword ptr[esp + 0x18]
		mov word ptr[ecx + 0x20a], ax
		popad
		jmp AddCopValueHook3ReturnAddress
	}
}

const JumpHook addCopValueHook3 = {
	0x004353d5,
	(DWORD)&AddCopValueHook3Function
};

static const DWORD SetCopValue600HookReturnAddress = 0x0043754e;

static __declspec(naked) void SetCopValue600HookFunction(void)
{
	__asm {
		pushad
		push ebx
		push esi
		call DispatchCopValueChangeEvent
		mov esi, dword ptr[esp + 0x4]
		mov word ptr[esi + 0x20a], ax
		popad
		jmp SetCopValue600HookReturnAddress
	}
}

const JumpHook setCopValue600Hook = {
	0x00437547,
	(DWORD)&SetCopValue600HookFunction
};

static const DWORD ExplodeCarHookReturnAddress = 0x0042715a;

static __declspec(naked) void ExplodeCarHookFunction(void)
{
	__asm {
		pushad
		push eax
		push edi
		call DispatchCopValueChangeEvent
		mov word ptr[esp + 0x1c], ax
		popad
		mov word ptr[edi + 0x20a], ax
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
		pushad
		push 0
		push ecx
		call DispatchCopValueChangeEvent
		mov ecx, dword ptr[esp + 0x18]
		mov word ptr[ecx + 0x20a], ax
		popad
		ret
	}
}

const JumpHook setCopValue0Hook = {
	0x00420b80,
	(DWORD)&SetCopValue0HookFunction
};

static __declspec(naked) void SetCopStarsHookFunction()
{
	__asm {
		pushad
		mov eax, dword ptr[esp + 0x24]
		push eax
		push ecx
		call DispatchCopValueChangeEventWithStars
		mov ecx, dword ptr[esp + 0x18]
		mov word ptr[ecx + 0x20a], ax
		popad
		ret 0x4
	}
}

const JumpHook setCopStarsHook = {
	0x00434cd0,
	(DWORD)&SetCopStarsHookFunction
};

bool CopValueChangeEvent::Init()
{
	Core::HookManager* hookManager = Core::HookManager::GetInstance();

	return hookManager->AddHook(copKillHook) &&
		hookManager->AddHook(addCopValueHook1) &&
		hookManager->AddHook(addCopValueHook2) &&
		hookManager->AddHook(addCopValueHook3) &&
		hookManager->AddHook(setCopValue600Hook) &&
		hookManager->AddHook(explodeCarHook) &&
		hookManager->AddHook(setCopValue0Hook) &&
		hookManager->AddHook(setCopStarsHook);
}
