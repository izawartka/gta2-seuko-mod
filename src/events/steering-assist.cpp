#include "steering-assist.h"
#include "../game/game.h"
#include "../hook-types/function-call-hook.h"

static bool __fastcall DispatchSteeringAssistEvent()
{
	SteeringAssistEvent event;
	Core::EventManager::GetInstance()->Dispatch(event);
	return event.GetDoAssist();
}

static __declspec(naked) void SteeringAssistHookFunction(void)
{
	__asm {
		pushad
		call DispatchSteeringAssistEvent
		test al, al
		popad
		jnz L_doAssist
		ret

	L_doAssist:
		call Game::Functions::ApplySteeringAssist
		ret
	}
}

const FunctionCallHook steeringAssistHook = {
	0x004a4607,
	(DWORD)&SteeringAssistHookFunction
};

bool SteeringAssistEvent::Init()
{
	return Core::HookManager::GetInstance()->AddHook(steeringAssistHook);
}
