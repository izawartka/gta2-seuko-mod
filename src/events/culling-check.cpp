#include "culling-check.h"
#include "../hook-types/function-multi-call-hook.h"

static bool __fastcall DispatchCullingCheckEvent(bool doCull)
{
	CullingCheckEvent event(doCull);
	Core::EventManager::GetInstance()->Dispatch(event);
	return event.GetModifiedDoCull();
}

static void __declspec(naked) SafelyReturn(void)
{
	__asm {
		pop ebx
		sub ebx, 5

		// DrawLeftTile / DrawTopTile group
		cmp ebx, 0x0046C661
		je L_DrawLeftTop
		cmp ebx, 0x0046C327
		je L_DrawLeftTop
		cmp ebx, 0x0046C495
		je L_DrawLeftTop
		cmp ebx, 0x0046D1D2
		je L_DrawLeftTop
		cmp ebx, 0x0046CE99
		je L_DrawLeftTop
		cmp ebx, 0x0046D006
		je L_DrawLeftTop

		// DrawBottomTile / DrawRightTile group
		cmp ebx, 0x0046DE19
		je L_DrawBottomRight
		cmp ebx, 0x0046DA31
		je L_DrawBottomRight
		cmp ebx, 0x0046DBFA
		je L_DrawBottomRight
		cmp ebx, 0x0046CC71
		je L_DrawBottomRight
		cmp ebx, 0x0046C87F
		je L_DrawBottomRight
		cmp ebx, 0x0046CA4D
		je L_DrawBottomRight

		// SlopTile1..4 group
		cmp ebx, 0x0046D581
		je L_Slop1to4
		cmp ebx, 0x0046D597
		je L_Slop1to4
		cmp ebx, 0x0046D711
		je L_Slop1to4
		cmp ebx, 0x0046D727
		je L_Slop1to4
		cmp ebx, 0x0046D8D3
		je L_Slop1to4
		cmp ebx, 0x0046D423
		je L_Slop1to4

		// SlopTile5..8 group
		cmp ebx, 0x004700E9
		je L_Slop5to8
		cmp ebx, 0x004702D9
		je L_Slop5to8
		cmp ebx, 0x004704C9
		je L_Slop5to8
		cmp ebx, 0x004706A9
		je L_Slop5to8

	L_DrawLeftTop:
		pop edi
		pop esi
		pop ebx
		add esp, 0x60
		ret 4

	L_DrawBottomRight:
		pop edi
		pop esi
		pop ebx
		add esp, 0x98
		ret 4

	L_Slop1to4:
		pop esi
		add esp, 0x10
		ret 4

	L_Slop5to8:
		pop esi
		pop ebp
		pop ebx
		ret 16
	}
}

static __declspec(naked) void CullingCheckHookFunction(void)
{
	__asm {
		pushad
		setz cl
		and ecx, 1
		call DispatchCullingCheckEvent
		test eax, eax
		jz L_doNotCull
		popad
		jmp SafelyReturn

	L_doNotCull:
		popad
		inc dword ptr [esp]
		ret
	}
}

const FunctionMultiCallHook cullingCheckHook = {
	std::vector<DWORD>{
		0x0046c661, // DrawLeftTile
		0x0046c327,
		0x0046c495,
		0x0046de19, // DrawBottomTile
		0x0046da31,
		0x0046dbfa,
		0x0046cc71, // DrawRightTile
		0x0046c87f,
		0x0046ca4d,
		0x0046d1d2, // DrawTopTile
		0x0046ce99,
		0x0046d006,
		0x0046d581, // SlopTile1
		0x0046d597,
		0x0046d711, // SlopTile2
		0x0046d727,
		0x0046d8d3, // SlopTile3
		0x0046d423, // SlopTile4
		0x004700e9, // SlopTile5
		0x004702d9, // SlopTile6
		0x004704c9, // SlopTile7
		0x004706a9  // SlopTile8
	},
	(DWORD)&CullingCheckHookFunction
};

bool CullingCheckEvent::Init()
{
	return Core::HookManager::GetInstance()->AddHook(cullingCheckHook);
}
