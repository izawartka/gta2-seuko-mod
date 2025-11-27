#pragma once
#include "../core/core.h"

/*
Replaces a pointer to a function at a given address with a pointer to a custom function.
@param address The address where the function pointer will be replaced.
@param hookFunction The address of the custom function to point to.
*/
class FunctionPtrHook : public Core::HookBase
{
public:
	FunctionPtrHook(DWORD address, DWORD hookFunction)
		: address(address), hookFunction(hookFunction) {
	}
	~FunctionPtrHook() = default;

	/*
	@param hookDef The FunctionPtrHook definition.
	@param originalFunction Optional pointer to store the original function address.
	*/
	static bool Hook(const FunctionPtrHook& hookDef, DWORD* originalFunction = nullptr) {
		if (originalFunction != nullptr) *originalFunction = *(DWORD*)hookDef.address;
		*(DWORD*)hookDef.address = hookDef.hookFunction;

		return true;
	}

	const DWORD address;
	const DWORD hookFunction;
};
