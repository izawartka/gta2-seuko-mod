#pragma once
#include "gta2_wrapper.h"
#include "keycode.h"

namespace Game {
	class Functions  
	{  
	public:  
		typedef void(__fastcall EachFrameDrawType)(Game* game, DWORD edx);  
		static constexpr EachFrameDrawType* EachFrameDraw = (EachFrameDrawType*)0x0045a5a0; 

		typedef void(__fastcall GameTickType)(Game* game, DWORD edx);
		static constexpr GameTickType* GameTick = (GameTickType*)0x0045c1f0;

		typedef void(__stdcall OnKeyUpDownType)(DWORD keyboard, KeyCode* keyCode);
		static constexpr OnKeyUpDownType* OnKeyUpDown = (OnKeyUpDownType*)0x0044bfb0;

		typedef void(__fastcall DrawUIType)(S10* s10, DWORD edx);
		static constexpr DrawUIType* DrawUI = (DrawUIType*)0x004ca440;

		typedef void(__stdcall DrawGTATextType)(WCHAR* str, SCR_f x, SCR_f y, int param_4, SCR_f scale, PALETTE_BASE* paletteBasePtr, int remap, SPRITE_INVISIBILITY spriteInvisibility, SCR_f param_9);
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
	};  
}
