#pragma once
#include "../core/core.h"

/*
Replaces multiple 5 bytes at given addresses with calls to a custom function.
@param adresses The addresses where the call instructions will be placed.
@param hookFunction The address of the custom function to call.
*/
class FunctionMultiCallHook : public Core::HookBase
{
public:
	FunctionMultiCallHook(const std::vector<DWORD>& adresses, DWORD hookFunction)
		: addresses(adresses), hookFunction(hookFunction) {
	}
	~FunctionMultiCallHook() = default;

	static bool Hook(const FunctionMultiCallHook& hookDef) {
		DWORD OldProtection = { 0 };
		for (const auto& originalCallAddress : hookDef.addresses) {
			bool success = VirtualProtectEx(GetCurrentProcess(), (LPVOID)originalCallAddress, 5, PAGE_EXECUTE_READWRITE, &OldProtection);
			if (!success) {
				return false;
			}
		}

		for (const auto& originalCallAddress : hookDef.addresses) {
			*(BYTE*)((LPBYTE)originalCallAddress) = 0xE8;
			DWORD offset = hookDef.hookFunction - originalCallAddress - 5;
			*(DWORD*)((LPBYTE)originalCallAddress + 1) = offset;
		}

		return true;
	}

	const std::vector<DWORD> addresses;
	const DWORD hookFunction;
};
