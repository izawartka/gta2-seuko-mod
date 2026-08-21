#include "align.h"

static constexpr Game::SCR_f SCREEN_VIRTUAL_WIDTH = Game::Utils::FromFloat(640.0f);
static constexpr Game::SCR_f SCREEN_VIRTUAL_HEIGHT = Game::Utils::FromFloat(480.0f);

Game::SCR_f UiModule::Utils::ScreenAlignHorizontal(Align align, Game::SCR_f offset)
{
	return ChildAlign(align, SCREEN_VIRTUAL_WIDTH) + offset;
}

Game::SCR_f UiModule::Utils::ScreenAlignVertical(Align align, Game::SCR_f offset)
{
	return ChildAlign(align, SCREEN_VIRTUAL_HEIGHT) + offset;
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
