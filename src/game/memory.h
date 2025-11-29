#pragma once
#include "gta2_wrapper.h"
#include "keycode.h"

namespace Game {
	class Memory {
	public:
		static Game* GetGame() { return (Game*)*(DWORD*)0x005eb4fc; }
		static Ped* GetPlayerPed() {
			Game* game = GetGame();
			if (!game) return nullptr;
			Player* player = game->CurrentPlayer;
			if (!player) return nullptr;
			return player->ped;
		}
		static KeyCode* GetKeyPressCode() { return (KeyCode*)0x005e8f8c; }
		static uint* GetKeyPressState() { return (uint*)0x005e8f90; }
		static S4* GetS4() { return (S4*)*(DWORD*)0x00671978; }
		static Style_S3* GetStyleS3() { return (Style_S3*)*(DWORD*)0x00670684; }
		static Cheats* GetCheats() { return (Cheats*)0x005ead51; }
		static GangRespectContainer* GetGangRespectContainer() { return (GangRespectContainer*)*(DWORD*)0x005eb898; }
		static short* GetDefaultFontId() { return (short*)0x00672f24; }
		static S15_Script* GetS15() { return (S15_Script*)*(DWORD*)0x006644bc; }
		static Menu* GetMenu() { return (Menu*)0x005ec070; }
		static CarsManager* GetCarsManager() { return (CarsManager*)*(DWORD*)0x005e4ca0; }
		static Camera* GetMainCamera() { return (Camera*)*(DWORD*)0x005e3cc4; }
		static bool* GetIsFullScreen() { return (bool*)0x00595014; }
		static int* GetFullScreenWidth() { return (int*)0x006732e4; }
		static int* GetFullScreenHeight() { return (int*)0x006732e0; }
		static int* GetWindowedWidth() { return (int*)0x00673578; }
		static int* GetWindowedHeight() { return (int*)0x006732e8; }
	};
}
