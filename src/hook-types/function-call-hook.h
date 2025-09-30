#pragma once
#include "../core/core.h"

/*
Replaces a function call with a call to a custom function.
@param originalCallAddress The address of the original function call instruction (the CALL opcode).
@param hookFunction The address of the custom function to call instead.
*/
class FunctionCallHook : public Core::HookBase
{
public:
	FunctionCallHook(const DWORD originalCallAddress, const DWORD hookFunction)
		: originalCallAddress(originalCallAddress), hookFunction(hookFunction) {
	}
	~FunctionCallHook() = default;

	static bool Hook(const FunctionCallHook& hookDef) {
		DWORD OldProtection = { 0 };
		bool success = VirtualProtectEx(GetCurrentProcess(), (LPVOID)hookDef.originalCallAddress, 5, PAGE_EXECUTE_READWRITE, &OldProtection);
		if (!success) {
			return false;
		}

		*(BYTE*)((LPBYTE)hookDef.originalCallAddress) = 0xE8;
		DWORD offset = hookDef.hookFunction - hookDef.originalCallAddress - 5;
		*(DWORD*)((LPBYTE)hookDef.originalCallAddress + 1) = offset;

		return true;
	}

	const DWORD originalCallAddress;
	const DWORD hookFunction;
};
