#pragma once
#include "../core/core.h"

/*
Replaces multiple calls of the same function with calls to a custom function.
@param originalCallAddresses The addresses of the original function call instructions (the CALL opcodes).
@param hookFunction The address of the custom function to call instead.
*/
class FunctionMultiCallHook : public Core::HookBase
{
public:
	FunctionMultiCallHook(const std::vector<DWORD>& originalCallAddresses, DWORD hookFunction)
		: originalCallAddresses(originalCallAddresses), hookFunction(hookFunction) {
	}
	~FunctionMultiCallHook() = default;

	static bool Hook(const FunctionMultiCallHook& hookDef) {
		DWORD OldProtection = { 0 };
		for (const auto& originalCallAddress : hookDef.originalCallAddresses) {
			bool success = VirtualProtectEx(GetCurrentProcess(), (LPVOID)originalCallAddress, 5, PAGE_EXECUTE_READWRITE, &OldProtection);
			if (!success) {
				return false;
			}
		}

		for (const auto& originalCallAddress : hookDef.originalCallAddresses) {
			*(BYTE*)((LPBYTE)originalCallAddress) = 0xE8;
			DWORD offset = hookDef.hookFunction - originalCallAddress - 5;
			*(DWORD*)((LPBYTE)originalCallAddress + 1) = offset;
		}

		return true;
	}

	const std::vector<DWORD> originalCallAddresses;
	const DWORD hookFunction;
};
