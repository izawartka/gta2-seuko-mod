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

		typedef void(__fastcall EachFrameDrawType)(Game* game, DWORD edx);  
		static constexpr EachFrameDrawType* EachFrameDraw = (EachFrameDrawType*)0x0045a5a0; 

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

		static void DrawQuad(uint16_t flags, DWORD sprite, GTAVertex* verticies, uint16_t flags2) {
			typedef void(__stdcall DrawQuadType)(uint16_t flags, DWORD sprite, GTAVertex* verticies, uint16_t flags2);
			static DrawQuadType* DrawQuadFunc = *(DrawQuadType**)0x005952c4;
			DrawQuadFunc(flags, sprite, verticies, flags2);
		}

		typedef Car*(__stdcall SpawnCarType)(int x, int y, int z, short rot, CAR_MODEL4 model);
		static constexpr SpawnCarType* SpawnCar = (SpawnCarType*)0x00426e10;

		typedef uint(__fastcall AddWeaponType)(Ped* ped, DWORD edx, WEAPON_INDEX weapon, uint ammo);
		static constexpr AddWeaponType* AddWeapon = (AddWeaponType*)0x0043e4b0;

		typedef uint(__stdcall CarAddWeaponType)(CAR_WEAPON type, uint ammo, Car* car);
		static constexpr CarAddWeaponType* CarAddWeapon = (CarAddWeaponType*)0x004cd820;

		typedef void(__fastcall CarAddRoofGunType)(Car* car);
		static constexpr CarAddRoofGunType* CarAddRoofGun = (CarAddRoofGunType*)0x0041fdf0;

		typedef void(__fastcall CarAddRoofTankTurretType)(Car* car);
		static constexpr CarAddRoofTankTurretType* CarAddRoofTankTurret = (CarAddRoofTankTurretType*)0x0041fda0;

		typedef void(__fastcall CarAddRoofWaterGunType)(Car* car);
		static constexpr CarAddRoofWaterGunType* CarAddRoofWaterGun = (CarAddRoofWaterGunType*)0x0041fd50;

		typedef void(__fastcall CarAddRoofAntennaType)(Car* car);
		static constexpr CarAddRoofAntennaType* CarAddRoofAntenna = (CarAddRoofAntennaType*)0x00425fd0;

		// also set car->fireState to 0 to fully extinguish
		typedef void(__fastcall ExtinguishCarType)(Car* car, DWORD edx);
		static constexpr ExtinguishCarType* ExtinguishCar = (ExtinguishCarType*)0x004bf070;

		// allows to run the engine even after the car exploded
		typedef void(__fastcall FixCarBrokenEngineType)(Car* car, DWORD edx);
		static constexpr FixCarBrokenEngineType* FixCarBrokenEngine = (FixCarBrokenEngineType*)0x00421570;

		typedef void(__fastcall ExplodeCarType)(Car* car, DWORD edx, EXPLOSION_SIZE explosionSize);
		static constexpr ExplodeCarType* ExplodeCar = (ExplodeCarType*)0x00426fa0;

		typedef void(__fastcall ShootFromWeaponType)(WEAPON_STRUCT* weapon, DWORD edx);
		static constexpr ShootFromWeaponType* ShootFromWeapon = (ShootFromWeaponType*)0x004d0530;

		typedef void(__fastcall SaveGameType)(S15_Script* s15, DWORD edx, const char* saveFileName);
		static constexpr SaveGameType* SaveGame = (SaveGameType*)0x0047ef40;
	};  
}
