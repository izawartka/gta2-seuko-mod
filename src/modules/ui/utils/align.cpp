#include "align.h"

static constexpr Game::SCR_f SCREEN_VIRTUAL_WIDTH = Game::Utils::FromFloat(640.0f);
static constexpr Game::SCR_f SCREEN_VIRTUAL_HEIGHT = Game::Utils::FromFloat(480.0f);
static constexpr float SCREEN_DEFAULT_WIDTH = 640.0f;
static constexpr float SCREEN_DEFAULT_HEIGHT = 480.0f;

static bool ProbeWSFRunning() {
	volatile DWORD transformAddress = *(DWORD*)0x004c72ab;
	return (transformAddress != 0xfff3a8b1);
}

Game::SCR_f UiModule::Utils::ScreenAlignHorizontal(Align align, Game::SCR_f offset)
{
	if (ProbeWSFRunning()) {
		float screenWidth = static_cast<float>(*Game::Memory::GetWindowedWidth());
		float screenHeight = static_cast<float>(*Game::Memory::GetWindowedHeight());
		float aspectRatio = screenWidth / screenHeight;

		float containerSize = aspectRatio * SCREEN_DEFAULT_HEIGHT;
		Game::SCR_f containerSizeScrf = Game::Utils::FromFloat(containerSize);

		return ChildAlign(align, containerSizeScrf) + offset;
	}
	else {
		return ChildAlign(align, SCREEN_VIRTUAL_WIDTH) + offset;
	}
}

Game::SCR_f UiModule::Utils::ScreenAlignVertical(Align align, Game::SCR_f offset)
{
	if (ProbeWSFRunning()) {
		return ChildAlign(align, SCREEN_VIRTUAL_HEIGHT) + offset;
	}
	else {
		float screenWidth = static_cast<float>(*Game::Memory::GetWindowedWidth());
		float screenHeight = static_cast<float>(*Game::Memory::GetWindowedHeight());
		float aspectRatio = screenWidth / screenHeight;

		float containerSize = SCREEN_DEFAULT_WIDTH / aspectRatio;
		Game::SCR_f containerSizeScrf = Game::Utils::FromFloat(containerSize);

		return ChildAlign(align, containerSizeScrf) + offset;
	}
}

Game::SCR_f UiModule::Utils::ChildAlign(Align align, Game::SCR_f containerSize)
{
	switch (align) {
	case Align::Start:
	default:
		return 0;
	case Align::Center:
		return containerSize / 2;
	case Align::End:
		return containerSize;
	}
}
