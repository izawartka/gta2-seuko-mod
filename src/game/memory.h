#pragma once
#include "gta2_wrapper.h"
#include "keycode.h"

namespace Game {
	class Memory
	{
	public:
		static constexpr Game* GetGame() { return (Game*)*(DWORD*)0x005eb4fc; }
		static constexpr KeyCode* GetKeyPressCode() { return (KeyCode*)0x005e8f8c; }
		static constexpr uint* GetKeyPressState() { return (uint*)0x005e8f90; }
	};
}
