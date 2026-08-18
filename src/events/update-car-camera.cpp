#include "update-car-camera.h"
#include "../game/game.h"
#include "../hook-types/function-multi-call-hook.h"

static bool __stdcall DispatchPreUpdateCarCameraEvent(
	Game::Camera* camera, 
	Game::Car* car, 
	Game::SCR_f* cameraX, 
	Game::SCR_f* cameraY, 
	Game::SCR_f* cameraZ
) {
	PreUpdateCarCameraEvent event(
		camera, car, cameraX, cameraY, cameraZ
	);
	Core::EventManager::GetInstance()->Dispatch(event);
	return event.GetDoUpdate();
}

static void __stdcall DispatchPostUpdateCarCameraEvent(
	Game::Camera* camera,
	Game::Car* car,
	Game::SCR_f* cameraX,
	Game::SCR_f* cameraY,
	Game::SCR_f* cameraZ
) {
	PostUpdateCarCameraEvent event(
		camera, car, cameraX, cameraY, cameraZ
	);
	Core::EventManager::GetInstance()->Dispatch(event);
}

static DWORD OriginalUpdateCarCameraFn = 0x0041ebf0; /// TODO: move to Game::Functions

static __declspec(naked) void UpdateCarCameraHookFunction(void)
{
	__asm {
		pushad
		push dword ptr [esp + 0x30]
		push dword ptr [esp + 0x30]
		push dword ptr [esp + 0x30]
		push dword ptr [esp + 0x30]
		push ecx
		call DispatchPreUpdateCarCameraEvent
		test al, al
		popad
		jnz L_doUpdate
		ret 0x10

	L_doUpdate:
		push ecx
		push dword ptr[esp + 0x14]
		push dword ptr[esp + 0x14]
		push dword ptr[esp + 0x14]
		push dword ptr[esp + 0x14]
		call OriginalUpdateCarCameraFn
		pop ecx
		pushad
		push dword ptr[esp + 0x30]
		push dword ptr[esp + 0x30]
		push dword ptr[esp + 0x30]
		push dword ptr[esp + 0x30]
		push ecx
		call DispatchPostUpdateCarCameraEvent
		popad
		ret 0x10
	}
}

const FunctionMultiCallHook updateCarCameraHook = {
	{0x0041f52f, 0x0041eec5},
	(DWORD)&UpdateCarCameraHookFunction
};

bool PreUpdateCarCameraEvent::Init()
{
	return Core::HookManager::GetInstance()->AddHook(updateCarCameraHook);
}

bool PostUpdateCarCameraEvent::Init()
{
	return Core::HookManager::GetInstance()->AddHook(updateCarCameraHook);
}
