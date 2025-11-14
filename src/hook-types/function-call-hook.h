#pragma once
#include "../core/core.h"

/*
Replaces 5 bytes at a given address with a call instruction to a custom function.
@param addresses The address where the call instruction will be placed.
@param hookFunction The address of the custom function to call.
*/
class FunctionCallHook : public Core::HookBase
{
public:
	FunctionCallHook(DWORD address, DWORD hookFunction)
		: addresses(address), hookFunction(hookFunction) {
	}
	~FunctionCallHook() = default;

	static bool Hook(const FunctionCallHook& hookDef) {
		DWORD OldProtection = { 0 };
		bool success = VirtualProtectEx(GetCurrentProcess(), (LPVOID)hookDef.addresses, 5, PAGE_EXECUTE_READWRITE, &OldProtection);
		if (!success) {
			return false;
		}

		*(BYTE*)((LPBYTE)hookDef.addresses) = 0xE8;
		DWORD offset = hookDef.hookFunction - hookDef.addresses - 5;
		*(DWORD*)((LPBYTE)hookDef.addresses + 1) = offset;

		return true;
	}

	const DWORD addresses;
	const DWORD hookFunction;
};
