#include "camera-follow-update.h"
#include "../hook-types/jump-hook.h"

static Game::CAMERA_FOLLOW_STATE __stdcall DispatchCameraFollowUpdateEvent(Game::Camera* camera, Game::CAMERA_FOLLOW_STATE followState)
{
	CameraFollowUpdateEvent event(camera, followState);
	Core::EventManager::GetInstance()->Dispatch(event);
	return event.GetModifiedFollowState();
}

static DWORD cameraFollowUpdateFollowReturnAddress = 0x0041f341;
static DWORD cameraFollowUpdateTeleportReturnAddress = 0x0041f31d;
static DWORD cameraFollowUpdateOtherReturnAddress = 0x0041f3c6;

static __declspec(naked) void CameraFollowUpdateHookFunction(void)
{
	__asm {
		mov eax, dword ptr[esi + 0x3c]
		pushad
		push eax
		push esi
		call DispatchCameraFollowUpdateEvent
		mov[esp + 0x1c], eax
		popad
		dec eax
		jz L_follow
		dec eax
		jnz L_other

		jmp cameraFollowUpdateTeleportReturnAddress

	L_follow:
		jmp cameraFollowUpdateFollowReturnAddress

	L_other:
		jmp cameraFollowUpdateOtherReturnAddress
	}
}

const JumpHook cameraFollowUpdateHook = {
	0x0041f310,
	(DWORD)&CameraFollowUpdateHookFunction
};

bool CameraFollowUpdateEvent::Init()
{
	return Core::HookManager::GetInstance()->AddHook(cameraFollowUpdateHook);
}
