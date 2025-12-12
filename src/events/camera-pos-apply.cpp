#include "camera-pos-apply.h"
#include "../hook-types/jump-hook.h"

static bool __stdcall DispatchCameraPosApplyEvent(Game::Camera* camera)
{
	CameraPosApplyEvent event(camera);
	Core::EventManager::GetInstance()->Dispatch(event);
	return event.GetDoApply();
}

static __declspec(naked) void CameraPosApplyHookFunction(void)
{
	__asm {
		pushad
		push esi
		call DispatchCameraPosApplyEvent
		test al, al
		popad
		jz L_skipApply
		mov eax, dword ptr[edx]
		mov dword ptr[esi], eax
		mov ecx, dword ptr[edx + 4]
		mov dword ptr[esi + 4], ecx
		mov eax, dword ptr[edx + 8]
		mov dword ptr[esi + 8], eax
		mov ecx, dword ptr[edx + 12]
		mov dword ptr[esi + 12], ecx

	L_skipApply:
		pop esi
		pop ecx
		ret
	}
}

const JumpHook cameraPosApplyHook = {
	0x0041f3e9,
	(DWORD)&CameraPosApplyHookFunction
};

bool CameraPosApplyEvent::Init()
{
	return Core::HookManager::GetInstance()->AddHook(cameraPosApplyHook);
}
