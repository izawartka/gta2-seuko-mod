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
		static constexpr S4* GetS4() { return (S4*)*(DWORD*)0x00671978; }
		static constexpr Style_S3* GetStyleS3() { return (Style_S3*)*(DWORD*)0x00670684; }
	};
}
