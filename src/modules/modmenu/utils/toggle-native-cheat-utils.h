#pragma once
#include "../common.h"
#include "../cheats/native-cheats-keeper.h"

namespace ModMenuModule::Utils::ToggleNativeCheat {
	enum class ToggleNativeCheatMode {
		UnchangedEnabled,
		UnchangedDisabled,
		DisabledEnabled,
		Cycle
	};

	const std::vector<ToggleNativeCheatMode>& GetAllToggleNativeCheatModes();
	NativeCheatState GetNextState(NativeCheatState currentState, ToggleNativeCheatMode mode);
	std::optional<NativeCheatState> ToggleNativeCheat(size_t cheatAddress, ToggleNativeCheatMode mode);
}
