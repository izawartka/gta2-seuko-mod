#pragma once
#include "mouse-position.h"

namespace MouseModule {
	struct MouseState {
		bool leftButtonDown = false;
		bool rightButtonDown = false;
		bool middleButtonDown = false;
		MousePosition position{ 0, 0 };
		bool isInWindow = false;
	};

}
