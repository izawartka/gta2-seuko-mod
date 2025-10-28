#pragma once
#include "gta2_wrapper.h"
#include "keycode.h"

namespace Game {
	class Memory
	{
	public:
		static constexpr Game* GetGame() { return (Game*)*(DWORD*)0x005eb4fc; }
		static constexpr Ped* GetPlayerPed() {
			Game* game = GetGame();
			if (!game) return nullptr;
			Player* player = game->CurrentPlayer;
			if (!player) return nullptr;
			return player->ped;
		}
		static constexpr KeyCode* GetKeyPressCode() { return (KeyCode*)0x005e8f8c; }
		static constexpr uint* GetKeyPressState() { return (uint*)0x005e8f90; }
		static constexpr S4* GetS4() { return (S4*)*(DWORD*)0x00671978; }
		static constexpr Style_S3* GetStyleS3() { return (Style_S3*)*(DWORD*)0x00670684; }
		static constexpr Cheats* GetCheats() { return (Cheats*)0x005ead51; }
		static constexpr GangRespectContainer* GetGangRespectContainer() { return (GangRespectContainer*)*(DWORD*)0x005eb898; }
		static short GetDefaultFontId() { return *(short*)0x00672f24; }
	};
}
