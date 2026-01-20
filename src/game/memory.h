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
		static bool* GetIsRendererLoaded() { return (bool*)0x00675358; }
		static DWORD GetRendererBase() { 
			DWORD initDllPtr = *(DWORD*)0x005952a8;
			if (!initDllPtr) return 0;
			return (initDllPtr - 0x00e053f0);
		}
		static SVideo** GetSVideoPtr() { return (SVideo**)0x00673d20; }
		static bool* GetIsFullScreen() { return (bool*)0x00595014; }
		static int* GetFullScreenWidth() { return (int*)0x006732e4; }
		static int* GetFullScreenHeight() { return (int*)0x006732e0; }
		static int* GetWindowedWidth() { return (int*)0x00673578; }
		static int* GetWindowedHeight() { return (int*)0x006732e8; }
		static int* GetScreenWidth() { return (int*)0x00673584; }
		static int* GetScreenHeight() { return (int*)0x00673474; }
		static int* GetCurrentlyDrawnMapLayerIndex() { return (int*)0x006633a0; }
		static MapBlocks* GetMapBlocks() { return (MapBlocks*)*(DWORD*)0x00662c08; }
		static int* GetTankCannonRoofOffsetY() { return (int*)0x005e4d64; }
		static int* GetJeepGunRoofOffsetY() { return (int*)0x005e500c; }
		static int* GetAntennaRoofOffsetY() { return (int*)0x005e5014; }
		static int* GetWaterCannonRoofOffsetY() { return (int*)0x005e4eb8; }
		static S33* GetS33() { return (S33*)*(DWORD*)0x00665788; }
		static TrafficManager* GetTrafficManager() { return (TrafficManager*)*(DWORD*)0x005e4ca4; }
		static int* GetCarsScale() { return (int*)0x005e4d4c; }
	};
}
