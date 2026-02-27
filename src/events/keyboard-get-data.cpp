#include "keyboard-get-data.h"
#include "../hook-types/jump-hook.h"

static void DispatchKeyboardGetDataEvent()
{
	Game::DWORD* dataCount = Game::Memory::GetDirectInputDeviceObjectDataCount();
	Game::DirectInput_DeviceObjectData* keyboardData = Game::Memory::GetDirectInputDeviceObjectData();
	Game::KeyCode keyCode = static_cast<Game::KeyCode>(keyboardData->ofs);
	bool isDown = (keyboardData->data & 0x80) != 0;
	KeyboardGetDataEvent event(*dataCount, keyCode, isDown);

	Core::EventManager::GetInstance()->Dispatch(event);

	*dataCount = event.GetModifiedDataCount();
	keyboardData->ofs = static_cast<DWORD>(event.GetModifiedKeyCode());
	keyboardData->data = event.GetModifiedIsDown() ? 0x80 : 0;
}

static DWORD KeyboardGetDataHookReturnAddress = 0x0044c1d7;

static __declspec(naked) void KeyboardGetDataHookFunction(void)
{
	__asm {
		pushad
		call DispatchKeyboardGetDataEvent
		popad
		mov dword ptr [esp + 0x18], eax
		jmp KeyboardGetDataHookReturnAddress
	}
}

const JumpHook keyboardGetDataHook = {
	0x0044c1c9,
	(DWORD)&KeyboardGetDataHookFunction
};

bool KeyboardGetDataEvent::Init()
{
	return Core::HookManager::GetInstance()->AddHook(keyboardGetDataHook);
}
