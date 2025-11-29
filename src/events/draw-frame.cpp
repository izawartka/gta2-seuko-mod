#include "draw-frame.h"
#include "../game/game.h"
#include "../hook-types/function-multi-call-hook.h"

static void DispatchPreDrawFrameEvent()
{
	PreDrawFrameEvent event;
	Core::EventManager::GetInstance()->Dispatch(event);
}

static void DispatchDrawFrameEvent()
{
	DrawFrameEvent event;
	Core::EventManager::GetInstance()->Dispatch(event);
}

static bool DispatchClearScreenEvent(bool doClear)
{
	ClearScreenEvent event(doClear);
	Core::EventManager::GetInstance()->Dispatch(event);
	return event.GetDoClear();
}

static void __stdcall DrawFrameWrapperHookFunction()
{
	DispatchPreDrawFrameEvent();
	Game::Functions::EachFrameDraw(Game::Memory::GetGame());
	DispatchDrawFrameEvent();
	Game::SVideo* sVideo = *Game::Memory::GetSVideoPtr();
	Game::Functions::FlipBuffers(sVideo);
	Game::Cheats* cheats = Game::Memory::GetCheats();
	bool doClearScreen = cheats->skipLid || cheats->skipTopTiles || cheats->skipBottomTiles || cheats->skipLeftTiles || cheats->skipRightTiles;
	if (DispatchClearScreenEvent(doClearScreen)) {
		Game::Functions::ClearScreen(sVideo, 0, 0, 0, 0, 0, sVideo->rect_right, sVideo->rect_bottom);
	}
}

const FunctionMultiCallHook drawFrameHook = {
	{0x0046278f, 0x00462b3f},
	(DWORD)&DrawFrameWrapperHookFunction
};

bool PreDrawFrameEvent::Init()
{
	return Core::HookManager::GetInstance()->AddHook(drawFrameHook);
}

bool DrawFrameEvent::Init()
{
	return Core::HookManager::GetInstance()->AddHook(drawFrameHook);
}

bool ClearScreenEvent::Init()
{
	return Core::HookManager::GetInstance()->AddHook(drawFrameHook);
}