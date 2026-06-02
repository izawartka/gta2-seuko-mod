#include "draw-player-name.h"
#include "../hook-types/function-call-hook.h"

static void __stdcall DispatchDrawPlayerNameEvent(Game::SCR_f* x, Game::SCR_f* y, Game::SCR_f* z)
{
	DrawPlayerNameEvent event({ *x, *y, *z });
	Core::EventManager::GetInstance()->Dispatch(event);

	Game::SCR_Vector3 modifiedPosition = event.GetModifiedPosition();
	*x = modifiedPosition.x;
	*y = modifiedPosition.y;
	*z = modifiedPosition.z;
}

static __declspec(naked) void DrawPlayerNameHookFunction(void)
{
	__asm {
		pushad
		mov eax, esp
		add eax, 0x2c
		push eax
		sub eax, 0x4
		push eax
		sub eax, 0x4
		push eax
		call DispatchDrawPlayerNameEvent
		popad
		jmp Game::Functions::WorldPointToScreen
	}
}

const FunctionCallHook drawPlayerNameHook = {
	0x004c7966,
	(DWORD)&DrawPlayerNameHookFunction
};

bool DrawPlayerNameEvent::Init()
{
	return Core::HookManager::GetInstance()->AddHook(drawPlayerNameHook);
}
