#include "mouse.h"
#include "../mouse-manager.h"

static bool CommonInit()
{
	if (!MouseModule::MouseManager::IsSafeToInitEvents()) {
		spdlog::error("Cannot initialize mouse event: MouseManager is not attached");
		return false;
	}

	return MouseModule::MouseManager::GetInstance()->MarkEventInitialized();
}

static void CommonDeinit()
{
	MouseModule::MouseManager::GetInstance()->MarkEventDeinitialized();
}

bool MouseModule::MouseButtonDownEvent::Init()
{
	return CommonInit();
}

void MouseModule::MouseButtonDownEvent::Deinit()
{
	CommonDeinit();
}

bool MouseModule::MouseButtonUpEvent::Init()
{
	return CommonInit();
}

void MouseModule::MouseButtonUpEvent::Deinit()
{
	CommonDeinit();
}

bool MouseModule::MouseMoveEvent::Init()
{
	return CommonInit();
}

void MouseModule::MouseMoveEvent::Deinit()
{
	CommonDeinit();
}

bool MouseModule::MouseLockedMoveEvent::Init()
{
	return CommonInit();
}

void MouseModule::MouseLockedMoveEvent::Deinit()
{
	CommonDeinit();
}

bool MouseModule::MouseLeaveEvent::Init()
{
	return CommonInit();
}

void MouseModule::MouseLeaveEvent::Deinit()
{
	CommonDeinit();
}

bool MouseModule::MouseEnterEvent::Init()
{
	return CommonInit();
}

void MouseModule::MouseEnterEvent::Deinit()
{
	CommonDeinit();
}
