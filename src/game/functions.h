#pragma once
#include "gta2_wrapper.h"
#include "keycode.h"

namespace Game {
	class Functions  
	{  
	public:  
		typedef void InitGameType();
		static constexpr InitGameType* InitGame = (InitGameType*)0x00461de0;

		typedef void DeinitGameType();
		static constexpr DeinitGameType* DeinitGame = (DeinitGameType*)0x00462060;

		typedef void(__fastcall EachFrameDrawType)(Game* gamePtr);  
		static constexpr EachFrameDrawType* EachFrameDraw = (EachFrameDrawType*)0x0045a5a0; 

		typedef void(__fastcall DrawMapLayerType)(S12* s12);
		// PreDrawMapLayerEvent won't be called.
		static constexpr DrawMapLayerType* DrawMapLayer = (DrawMapLayerType*)0x004720e0;

		typedef void(__fastcall DrawTileBlockType)(S12* s12, int param_2, int* x, int* y);
		static constexpr DrawTileBlockType* DrawTileBlock = (DrawTileBlockType*)0x00471f20;

		typedef void(__fastcall GameTickType)(Game* game, DWORD edx);
		static constexpr GameTickType* GameTick = (GameTickType*)0x0045c1f0;

		typedef void(__stdcall OnKeyUpDownType)(DWORD keyboard, KeyCode* keyCode);
		static constexpr OnKeyUpDownType* OnKeyUpDown = (OnKeyUpDownType*)0x0044bfb0;

		typedef void(__fastcall DrawUIType)(S10* s10, DWORD edx);
		static constexpr DrawUIType* DrawUI = (DrawUIType*)0x004ca440;

		typedef SCR_f(__stdcall GetGTATextWidthType)(const wchar_t* text, short fontId);
		static constexpr GetGTATextWidthType* GetGTATextWidth = (GetGTATextWidthType*)0x004cb0c0;

		typedef void(__stdcall DrawGTATextType)(const wchar_t* str, SCR_f x, SCR_f y, short fontId, SCR_f scale, PALETTE_BASE* paletteBasePtr, int remap, SPRITE_INVISIBILITY spriteInvisibility, SCR_f param_9);
		static constexpr DrawGTATextType* DrawGTAText = (DrawGTATextType*)0x004cc100;

		typedef bool* (__fastcall SetSpritePositionType)(Sprite* sprite, DWORD edx, SCR_f x, SCR_f y, SCR_f z);
		static constexpr SetSpritePositionType* SetSpritePosition = (SetSpritePositionType*)0x00420600;

		typedef uint(__fastcall GetSpriteIndexType)(Style_S3* style, DWORD edx, SPRITE_TYPE spriteType, uint spriteId);
		static constexpr GetSpriteIndexType* GetSpriteIndex = (GetSpriteIndexType*)0x004bf2a0;

		typedef void* (__fastcall GetSpriteTextureType)(S4* s4, DWORD edx, SPRITE_TYPE spriteType, uint spriteId, PALETTE_BASE paletteBase, uint remap);
		static constexpr GetSpriteTextureType* GetSpriteTexture = (GetSpriteTextureType*)0x004c2ac0;

		typedef SpriteEntry* (__fastcall GetSpriteEntryType)(Style_S3* style, DWORD edx, uint spriteIndex);
		static constexpr GetSpriteEntryType* GetSpriteEntry = (GetSpriteEntryType*)0x004bf280;

		typedef void(__stdcall DrawQuadType)(uint32_t flags, DWORD sprite, GTAVertex* verticies, uint16_t flags2);
		static void DrawQuad(uint32_t flags, DWORD sprite, GTAVertex* verticies, uint16_t flags2) {
			static DrawQuadType* DrawQuadFunc = *(DrawQuadType**)0x005952c4;
			DrawQuadFunc(flags, sprite, verticies, flags2);
		}

		typedef void(__stdcall FlipBuffersType)(SVideo* sVideo);
		static void FlipBuffers(SVideo* sVideo) {
			static FlipBuffersType* FlipBuffersFunc = *(FlipBuffersType**)0x00595044;
			FlipBuffersFunc(sVideo);
		}

		typedef void(__stdcall ClearScreenType)(SVideo* sVideo, DWORD param_2, DWORD param_3, DWORD param_4, DWORD param_5, DWORD param_6, DWORD param_7, DWORD param_8);
		static void ClearScreen(SVideo* sVideo, DWORD param_2, DWORD param_3, DWORD param_4, DWORD param_5, DWORD param_6, DWORD param_7, DWORD param_8) {
			static ClearScreenType* ClearScreenFunc = *(ClearScreenType**)0x00595064;
			ClearScreenFunc(sVideo, param_2, param_3, param_4, param_5, param_6, param_7, param_8);
		}
		
		typedef BareGameObject* (__fastcall SpawnObjectType)(S33* s33, DWORD edx, OBJECT_TYPE type, SCR_f x, SCR_f y, SCR_f z, short rot);
		static constexpr SpawnObjectType* SpawnObject = (SpawnObjectType*)0x004852e0;

		typedef Car* (__fastcall SpawnCarType)(TrafficManager* trafficManager, DWORD edx, SCR_f x, SCR_f y, SCR_f z, int rot, CAR_MODEL4 model, int scale);
		static constexpr SpawnCarType* SpawnCar = (SpawnCarType*)0x00426ac0;

		typedef WEAPON_STRUCT* (__stdcall GetCarWeaponStructType)(Car* car, CAR_WEAPON_INDEX weapon);
		static constexpr GetCarWeaponStructType* GetCarWeaponStruct = (GetCarWeaponStructType*)0x004cd7f0;

		typedef WEAPON_STRUCT* (__stdcall CreateCarWeaponStructType)(CAR_WEAPON_INDEX type, Car* car, short ammo);
		// also requires calling some player weapon list related functions if there is a ped inside, check 0x004cd820 for details
		static constexpr CreateCarWeaponStructType* CreateCarWeaponStruct = (CreateCarWeaponStructType*)0x004cd7b0;

		typedef uint(__fastcall AddWeaponType)(Ped* ped, DWORD edx, WEAPON_INDEX weapon, uint ammo);
		static constexpr AddWeaponType* AddWeapon = (AddWeaponType*)0x0043e4b0;

		typedef void(__stdcall CarAddWeaponType)(CAR_WEAPON_INDEX type, uint ammo, Car* car);
		static constexpr CarAddWeaponType* CarAddWeapon = (CarAddWeaponType*)0x004cd820;

		typedef void(__fastcall CarAddRoofGunType)(Car* car);
		static constexpr CarAddRoofGunType* CarAddRoofGun = (CarAddRoofGunType*)0x0041fdf0;

		typedef void(__fastcall CarAddRoofTankTurretType)(Car* car);
		static constexpr CarAddRoofTankTurretType* CarAddRoofTankTurret = (CarAddRoofTankTurretType*)0x0041fda0;

		typedef void(__fastcall CarAddRoofWaterGunType)(Car* car);
		static constexpr CarAddRoofWaterGunType* CarAddRoofWaterGun = (CarAddRoofWaterGunType*)0x0041fd50;

		typedef void(__fastcall CarAddRoofAntennaType)(Car* car);
		static constexpr CarAddRoofAntennaType* CarAddRoofAntenna = (CarAddRoofAntennaType*)0x00425fd0;

		typedef void(__fastcall ExtinguishCarType)(Car* car);
		// also set car->fireState to 0 to fully extinguish
		static constexpr ExtinguishCarType* ExtinguishCar = (ExtinguishCarType*)0x004bf070;

		typedef void(__fastcall FixCarBrokenEngineType)(Car* car);
		// allows to run the engine even after the car exploded
		static constexpr FixCarBrokenEngineType* FixCarBrokenEngine = (FixCarBrokenEngineType*)0x00421570;

		typedef void(__fastcall TurnOnCarBrakeLightsType)(Car* car);
		static constexpr TurnOnCarBrakeLightsType* TurnOnCarBrakeLights = (TurnOnCarBrakeLightsType*)0x004213d0;

		typedef void(__fastcall TurnOnCarLightsType)(Car* car);
		static constexpr TurnOnCarLightsType* TurnOnCarLights = (TurnOnCarLightsType*)0x00425590;

		typedef void(__fastcall ExplodeCarType)(Car* car, DWORD edx, EXPLOSION_SIZE explosionSize);
		static constexpr ExplodeCarType* ExplodeCar = (ExplodeCarType*)0x00426fa0;

		typedef void(__fastcall ShootFromWeaponType)(WEAPON_STRUCT* weapon, DWORD edx);
		static constexpr ShootFromWeaponType* ShootFromWeapon = (ShootFromWeaponType*)0x004d0530;

		typedef void(__fastcall SaveGameType)(S15_Script* s15, DWORD edx, const char* saveFileName);
		static constexpr SaveGameType* SaveGame = (SaveGameType*)0x0047ef40;

		typedef SCR_f* (__fastcall ScaleScrfType)(SCR_f* value, SCR_f* target, int* scalar);
		static constexpr ScaleScrfType* ScaleScrf = (ScaleScrfType*)0x00401bd0;

		typedef void(__fastcall FindMaxZType)(MapBlocks* mapBlocks, DWORD edx, SCR_f* outZ, SCR_f x, SCR_f y);
		static constexpr FindMaxZType* FindMaxZ = (FindMaxZType*)0x0046a420;

		typedef void(__fastcall TeleportCarType)(Car* car, DWORD edx, SCR_f x, SCR_f y, SCR_f z);
		static constexpr TeleportCarType* TeleportCar = (TeleportCarType*)0x004207f0;

		typedef void(__fastcall TeleportGameObjectType)(GameObject* gameObject, DWORD edx, SCR_f x, SCR_f y, SCR_f z);
		static constexpr TeleportGameObjectType* TeleportGameObject = (TeleportGameObjectType*)0x00491e00;

		typedef void(__fastcall TeleportPedType)(Ped* ped, DWORD edx, SCR_f x, SCR_f y);
		// z is auto-calculated based on the ground level
		static constexpr TeleportPedType* TeleportPed = (TeleportPedType*)0x00435c80;

		typedef uint(__fastcall GivePowerUpType)(Player* player, DWORD edx, POWERUP_TYPE type);
		static constexpr GivePowerUpType* GivePowerUp = (GivePowerUpType*)0x004a5780;

		typedef void ToggleGamePauseType();
		static constexpr ToggleGamePauseType* ToggleGamePause = (ToggleGamePauseType*)0x0045baa0;

		typedef void UnpauseGameType();
		// Does not send GameUnpauseEvent
		static constexpr UnpauseGameType* UnpauseGame = (UnpauseGameType*)0x0045ba60;

		typedef void HideCursorType();
		// Does not send HideCursorEvent
		static constexpr HideCursorType* HideCursor = (HideCursorType*)0x004cb530;

		typedef void ShowCursorType();
		// Does not send ShowCursorEvent
		static constexpr ShowCursorType* ShowCursor = (ShowCursorType*)0x004cb550;

		typedef bool InitMouseExclusiveType();
		// Does not send MouseExclusiveModeChangeEvent
		static constexpr InitMouseExclusiveType* InitMouseExclusive = (InitMouseExclusiveType*)0x004cad30;

		typedef void DeinitMouseExclusiveType();
		static constexpr DeinitMouseExclusiveType* DeinitMouseExclusive = (DeinitMouseExclusiveType*)0x004cadb0;

		typedef void(__fastcall ApplySteeringAssistType)(CarPhysics* carPhysics, DWORD edx);
		static constexpr ApplySteeringAssistType* ApplySteeringAssist = (ApplySteeringAssistType*)0x004a17c0;

		typedef Ped* (__fastcall GetCurrentPedType)(Player* player);
		static constexpr GetCurrentPedType* GetCurrentPed = (GetCurrentPedType*)0x004a5150;

		typedef void(__fastcall UnlockAllLevelsType)(S28* s28);
		static constexpr UnlockAllLevelsType* UnlockAllLevels = (UnlockAllLevelsType*)0x004a8b00;

		typedef Camera* (__fastcall GetPlayerCurrentCameraType)(Player* player);
		static constexpr GetPlayerCurrentCameraType* GetPlayerCurrentCamera = (GetPlayerCurrentCameraType*)0x00434900;

		typedef void(__fastcall WorldPointToScreenType)(Camera* camera, DWORD edx, SCR_f x, SCR_f y, SCR_f z, SCR_f** outX, SCR_f** outY);
		static constexpr WorldPointToScreenType* WorldPointToScreen = (WorldPointToScreenType*)0x0040cfc0;

		typedef GangRespect* (__fastcall GetFirstGangRespectType)(GangRespectContainer* gangRespectContainer);
		static constexpr GetFirstGangRespectType* GetFirstGangRespect = (GetFirstGangRespectType*)0x0045dd60;

		typedef GangRespect* (__fastcall GetNextGangRespectType)(GangRespectContainer* gangRespectContainer);
		static constexpr GetNextGangRespectType* GetNextGangRespect = (GetNextGangRespectType*)0x0045ddb0;

		typedef wchar_t* (__fastcall GetGangNameType)(GangRespect* gangRespect);
		static constexpr GetGangNameType* GetGangName = (GetGangNameType*)0x0045dd20;

		typedef wchar_t* (__fastcall GetCarModelNameType)(Car* car);
		static constexpr GetCarModelNameType* GetCarModelName = (GetCarModelNameType*)0x0041f700;

		typedef Ped* (__stdcall GetPedByIdType)(int);
		static constexpr GetPedByIdType* GetPedById = (GetPedByIdType*)0x0043ae10;

		typedef Ped* (__stdcall CreatePedType)(void);
		static constexpr CreatePedType* CreatePed = (CreatePedType*)0x0043df60;

		typedef void(__fastcall CreatePed2Type)(S17_Mission* mission, DWORD edx, Ped* ped);
		static constexpr CreatePed2Type* CreatePed2 = (CreatePed2Type*)0x00476d20;

		typedef void(__fastcall PutPedInCarType)(Ped* ped, DWORD edx, Car* car);
		static constexpr PutPedInCarType* PutPedInCar = (PutPedInCarType*)0x00436070;

		typedef void(__fastcall MakeCarDriveable1Type)(Car* car, DWORD edx, int eq5);
		static constexpr MakeCarDriveable1Type* MakeCarDriveable1 = (MakeCarDriveable1Type*)0x00421560;

		typedef void(__fastcall MakeCarDriveable2Type)(Car* car, DWORD edx);
		static constexpr MakeCarDriveable2Type* MakeCarDriveable2 = (MakeCarDriveable2Type*)0x00421510;

		typedef void(__fastcall MakeCarDriveable3Type)(CarManager4_S1* carManager, DWORD edx, Car* car);
		static constexpr MakeCarDriveable3Type* MakeCarDriveable3 = (MakeCarDriveable3Type*)0x0042a9d0;

		typedef void(__fastcall MakeCarDriveable4Type)(Car* car, DWORD edx);
		static constexpr MakeCarDriveable4Type* MakeCarDriveable4 = (MakeCarDriveable4Type*)0x00425dd0;

		typedef Ped* (__stdcall SpawnPedType)(SCR_f x, SCR_f y, SCR_f z, PED_REMAP remap, short param_5);
		static constexpr SpawnPedType* SpawnPed = (SpawnPedType*)0x0043db40;

		typedef void(__fastcall PedGroupCreateType)(Ped* ped, DWORD edx, byte memberCount);
		static constexpr PedGroupCreateType* PedGroupCreate = (PedGroupCreateType*)0x00440350;

		typedef void(__fastcall ChangePedGroupLeaderType)(Ped* newLeader, DWORD edx, Ped* oldLeader);
		static constexpr ChangePedGroupLeaderType* PedGroupSetLeader = (ChangePedGroupLeaderType*)0x00435490;

		typedef void(__fastcall PedGroupAddPedType)(PedGroup* group, DWORD edx, Ped* ped);
		static constexpr PedGroupAddPedType* PedGroupAddPed = (PedGroupAddPedType*)0x00404c90;

		typedef void(__fastcall PedSetObjectiveType)(Ped* ped, DWORD edx, PED_OBJECTIVE objective, ushort timer);
		static constexpr PedSetObjectiveType* PedSetObjective = (PedSetObjectiveType*)0x0043bbc0;

		typedef void(__fastcall SetPedWeaponType)(Ped* ped, DWORD edx, WEAPON_INDEX weapon);
		static constexpr SetPedWeaponType* SetPedWeapon = (SetPedWeaponType*)0x0043d830;

		typedef Car* (__fastcall GetNearestCarForEnterType)(TrafficManager* trafficManager, DWORD edx, Sprite* pedSprite, DWORD param_3);
		static constexpr GetNearestCarForEnterType* GetNearestCarForEnter = (GetNearestCarForEnterType*)0x00424e70;

		typedef bool(__fastcall IsCarATrainType)(Car* car);
		static constexpr IsCarATrainType* IsCarATrain = (IsCarATrainType*)0x00403ba0;

		typedef void(__fastcall UpdatePedStatesFromObjectiveType)(Ped* ped, DWORD edx, PED_OBJECTIVE objective, uint timer);
		static constexpr UpdatePedStatesFromObjectiveType* UpdatePedStatesFromObjective = (UpdatePedStatesFromObjectiveType*)0x00436920;
	};  
}
