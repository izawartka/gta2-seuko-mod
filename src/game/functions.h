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

		typedef void(__stdcall DrawGTATextType)(WCHAR* str, SCR_f x, SCR_f y, int param_4, SCR_f scale, S4_ENUM1* param_6, int param_7, SPRITE_INVISIBILITY spriteInvisibility, SCR_f param_9);
		static constexpr DrawGTATextType* DrawGTAText = (DrawGTATextType*)0x004cc100;
	};  
}
