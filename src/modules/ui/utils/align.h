#pragma once
#include "../common.h"

namespace UiModule::Utils {
	enum class Align {
		Start,
		Center,
		End
	};

	Game::SCR_f ScreenAlignHorizontal(Align align, Game::SCR_f offset);
	Game::SCR_f ScreenAlignVertical(Align align, Game::SCR_f offset);
	Game::SCR_f ChildAlign(Align align, Game::SCR_f containerSize);
}
