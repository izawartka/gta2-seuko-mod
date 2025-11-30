#include "../hook-types/function-call-hook.h"
#include "../hook-types/function-ptr-hook.h"
#include "renderer.h"

static void DispatchRendererLoadEvent()
{
	RendererLoadEvent event;
	Core::EventManager::GetInstance()->Dispatch(event);
}

static DWORD rendererLoadOriginalFunction = 0x004cae50;

static __declspec(naked) void RendererLoadHookFunction(void)
{
	__asm {
		call rendererLoadOriginalFunction
		call DispatchRendererLoadEvent
		ret
	}
}

const FunctionCallHook rendererLoadHook = {
	0x004cc778,
	(DWORD)&RendererLoadHookFunction
};

bool RendererLoadEvent::Init()
{
	return Core::HookManager::GetInstance()->AddHook(rendererLoadHook);
}

static void __stdcall DispatchDrawTileEvent(uint32_t flags, Game::GTAVertex*& vertices)
{
	RendererDrawTileEvent event(flags, vertices);
	Core::EventManager::GetInstance()->Dispatch(event);
}

DWORD drawTileOriginalFunction = 0;

static __declspec(naked) void DrawTileHookFunction(void)
{
	__asm {
		push esp
		add [esp], 0xc
		push[esp + 0x8]
		call DispatchDrawTileEvent

		jmp drawTileOriginalFunction
	}
}

const FunctionPtrHook drawTileHook = {
	0x005952bc,
	(DWORD)&DrawTileHookFunction
};

bool RendererDrawTileEvent::Init()
{
	if (!*Game::Memory::GetIsRendererLoaded()) return false;

	return Core::HookManager::GetInstance()->AddHook(drawTileHook, &drawTileOriginalFunction);
}

static void __stdcall DispatchDrawQuadEvent(uint32_t flags, Game::GTAVertex*& vertices)
{
	RendererDrawQuadEvent event(flags, vertices);
	Core::EventManager::GetInstance()->Dispatch(event);
}

DWORD drawQuadOriginalFunction = 0;

static __declspec(naked) void DrawQuadHookFunction(void)
{
	__asm {
		push esp
		add [esp], 0xc
		push [esp+0x8]
		call DispatchDrawQuadEvent

		jmp drawQuadOriginalFunction
	}
}

const FunctionPtrHook drawQuadHook = {
	0x005952c4,
	(DWORD)&DrawQuadHookFunction
};

bool RendererDrawQuadEvent::Init()
{
	if (!*Game::Memory::GetIsRendererLoaded()) return false;

	return Core::HookManager::GetInstance()->AddHook(drawQuadHook, &drawQuadOriginalFunction);
}

static void __stdcall DispatchDrawTriangleEvent(uint32_t flags, Game::GTAVertex*& vertices)
{
	RendererDrawTriangleEvent event(flags, vertices);
	Core::EventManager::GetInstance()->Dispatch(event);
}

DWORD drawTriangleOriginalFunction = 0;

static __declspec(naked) void DrawTriangleHookFunction(void)
{
	__asm {
		push esp
		add [esp], 0xc
		push[esp + 0x8]
		call DispatchDrawTriangleEvent

		jmp drawTriangleOriginalFunction
	}
}

const FunctionPtrHook drawTriangleHook = {
	0x005952d0,
	(DWORD)&DrawTriangleHookFunction
};

bool RendererDrawTriangleEvent::Init()
{
	if (!*Game::Memory::GetIsRendererLoaded()) return false;
	return Core::HookManager::GetInstance()->AddHook(drawTriangleHook, &drawTriangleOriginalFunction);
}
