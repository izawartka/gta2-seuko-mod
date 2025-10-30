#include "try-shoot-weapon.h"
#include "../hook-types/multi-jump-hook.h"

static void __fastcall DispatchTryShootWeaponEvent(Game::WEAPON_STRUCT* weapon)
{
	TryShootWeaponEvent event(weapon);
	Core::EventManager::GetInstance()->Dispatch(event);
}

static __declspec(naked) void TryShootWeaponHookFunction(void)
{
	__asm {
		push ecx
		push edx
		push esi
		call DispatchTryShootWeaponEvent
		pop esi
		pop edx
		pop ecx
		jmp Game::Functions::ShootFromWeapon
	}
}

const MultiJumpHook tryShootWeaponHook = {
	std::vector<DWORD>{ 0x004452f6, 0x004452c8 },
	(DWORD)&TryShootWeaponHookFunction
};

bool TryShootWeaponEvent::Init()
{
	return Core::HookManager::GetInstance()->AddHook(tryShootWeaponHook);
}
