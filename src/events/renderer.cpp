#include "renderer.h"
#include "../hook-types/function-call-hook.h"
#include "../hook-types/function-ptr-hook.h"

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

static Game::GTAVertex* __stdcall DispatchDrawTileEvent(uint32_t flags, Game::GTAVertex* vertices)
{
	RendererDrawTileEvent event(flags, vertices);
	Core::EventManager::GetInstance()->Dispatch(event);
	return event.GetVertices();
}

DWORD drawTileOriginalFunction = 0;

static __declspec(naked) void DrawTileHookFunction(void)
{
	__asm {
		push [esp + 0xc]
		push [esp + 0x8]
		call DispatchDrawTileEvent
		mov	[esp + 0xc], eax

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

static Game::GTAVertex* __stdcall DispatchDrawQuadEvent(uint32_t flags, Game::GTAVertex* vertices)
{
	RendererDrawQuadEvent event(flags, vertices);
	Core::EventManager::GetInstance()->Dispatch(event);
	return event.GetVertices();
}

DWORD drawQuadOriginalFunction = 0;

static __declspec(naked) void DrawQuadHookFunction(void)
{
	__asm {
		push [esp + 0xc]
		push [esp + 0x8]
		call DispatchDrawQuadEvent
		mov [esp + 0xc], eax

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

static Game::GTAVertex* __stdcall DispatchDrawTriangleEvent(uint32_t flags, Game::GTAVertex* vertices)
{
	RendererDrawTriangleEvent event(flags, vertices);
	Core::EventManager::GetInstance()->Dispatch(event);
	return event.GetVertices();
}

DWORD drawTriangleOriginalFunction = 0;

static __declspec(naked) void DrawTriangleHookFunction(void)
{
	__asm {
		push [esp + 0xc]
		push [esp + 0x8]
		call DispatchDrawTriangleEvent
		mov [esp + 0xc], eax

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
