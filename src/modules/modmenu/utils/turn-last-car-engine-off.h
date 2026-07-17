#pragma once
#include "../common.h"

namespace ModMenuModule::Utils {
	enum class TurnLastCarEngineOffResult {
		Success,
		AlreadyOff,
		NoLastCar,
	};

	TurnLastCarEngineOffResult TurnLastCarEngineOff();
	TurnLastCarEngineOffResult TurnLastCarEngineOffWithToast(bool showSuccessToast = true);
}
