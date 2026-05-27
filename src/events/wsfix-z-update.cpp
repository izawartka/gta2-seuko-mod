#include "wsfix-z-update.h"
#include "../hook-types/jump-hook.h"
#include "../hook-types/function-call-hook.h"

static const DWORD wsfixHookAddress = 0x004a80cd;
static const DWORD wsfixHookOriginalBytes = 0x00002fb9;
static const DWORD wsfixHookFunctionAddressPtr = 0x004a80ce;
static const DWORD returnAddress = 0x004a80d2;
static const DWORD hook2OriginalFunctionAddress = 0x004a6100;

static bool firstHookExecuted = false;
static Game::Camera* currentCamera = nullptr;
static Game::SCR_f lastZOffset = 0;
static DWORD wsfixHookFunctionAddress = 0;

static void __fastcall DispatchWSFixZUpdateEvent()
{
	bool hasWSFix = wsfixHookFunctionAddress != 0;
	Game::SCR_f zOffset = hasWSFix ? lastZOffset : 0;

	WSFixZUpdateEvent event(hasWSFix, zOffset);
	Core::EventManager::GetInstance()->Dispatch(event);
}

static __declspec(naked) void WSFixZUpdateHookFunction(void)
{
	__asm {
		lea edi, [ebp + 0x14c]
		mov byte ptr firstHookExecuted, 1

		push eax
		mov eax, dword ptr [wsfixHookAddress]
		mov eax, dword ptr [eax]
		cmp eax, dword ptr [wsfixHookOriginalBytes]
		pop eax

		je L_noWSF

		push eax
		mov dword ptr [currentCamera], esi
		mov eax, dword ptr [esi + 0x8]
		mov dword ptr [lastZOffset], eax
		mov eax, dword ptr [wsfixHookFunctionAddressPtr]
		mov eax, dword ptr [eax]
		add eax, dword ptr [returnAddress]
		mov dword ptr [wsfixHookFunctionAddress], eax
		pop eax

		jmp wsfixHookFunctionAddress

	L_noWSF:
		mov ecx, 0x2f
		mov dword ptr [wsfixHookFunctionAddress], 0

		jmp returnAddress
	}
}

static __declspec(naked) void WSFixZUpdateHookFunction2(void)
{
	__asm {
		pushad
		mov al, byte ptr [firstHookExecuted]
		test al, al
		jz L_finish

		mov eax, dword ptr [wsfixHookFunctionAddress]
		test eax, eax
		jz L_finish

		mov eax, dword ptr[currentCamera]
		mov eax, dword ptr[eax + 0x8]
		sub eax, dword ptr[lastZOffset]
		mov dword ptr[lastZOffset], eax

	L_finish:
		call DispatchWSFixZUpdateEvent
		popad
		call hook2OriginalFunctionAddress
		ret
	}
}

const JumpHook wsfixZUpdateHook = {
	0x004a80c7,
	(DWORD)&WSFixZUpdateHookFunction
};

const FunctionCallHook wsfixZUpdateHook2 = {
	0x004a80d7,
	(DWORD)&WSFixZUpdateHookFunction2
};

bool WSFixZUpdateEvent::Init()
{
	Core::HookManager* hookManager = Core::HookManager::GetInstance();

	return hookManager->AddHook(wsfixZUpdateHook) && hookManager->AddHook(wsfixZUpdateHook2);
}
