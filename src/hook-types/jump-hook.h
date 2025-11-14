#pragma once
#include "../core/core.h"

/*
Replaces 5 bytes at a given address with a jump instruction to a specified address.
@param address The address where the jump instruction will be placed.
@param jumpAddress The address to jump to.
*/
class JumpHook : public Core::HookBase
{
public:
	JumpHook(DWORD address, DWORD jumpAddress)
		: address(address), jumpAddress(jumpAddress) {
	}
	~JumpHook() = default;

	static bool Hook(const JumpHook& hookDef) {
		DWORD OldProtection = { 0 };
		bool success = VirtualProtectEx(GetCurrentProcess(), (LPVOID)hookDef.address, 5, PAGE_EXECUTE_READWRITE, &OldProtection);
		if (!success) {
			return false;
		}

		*(BYTE*)((LPBYTE)hookDef.address) = 0xE9;
		DWORD offset = hookDef.jumpAddress - hookDef.address - 5;
		*(DWORD*)((LPBYTE)hookDef.address + 1) = offset;

		return true;
	}

	const DWORD address;
	const DWORD jumpAddress;
};
