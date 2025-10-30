#pragma once
#include "../core/core.h"

/*
Replaces multiple 5 bytes at given addresses with jumps to the specified jumpAddress
@param addresses The addresses where the jump instructions will be placed.
@param jumpAddress The address to jump to.
*/
class MultiJumpHook : public Core::HookBase
{
public:
	MultiJumpHook(const std::vector<DWORD>& addresses, DWORD jumpAddress)
		: addresses(addresses), jumpAddress(jumpAddress) {
	}
	~MultiJumpHook() = default;

	static bool Hook(const MultiJumpHook& hookDef) {
		DWORD OldProtection = { 0 };
		for (const auto& originalCallAddress : hookDef.addresses) {
			bool success = VirtualProtectEx(GetCurrentProcess(), (LPVOID)originalCallAddress, 5, PAGE_EXECUTE_READWRITE, &OldProtection);
			if (!success) {
				return false;
			}
		}

		for (const auto& address : hookDef.addresses) {
			*(BYTE*)((LPBYTE)address) = 0xE9;
			DWORD offset = hookDef.jumpAddress - address - 5;
			*(DWORD*)((LPBYTE)address + 1) = offset;
		}

		return true;
	}

	const std::vector<DWORD> addresses;
	const DWORD jumpAddress;
};
