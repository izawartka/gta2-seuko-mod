#include "toggle-native-cheat-utils.h"

const std::vector<ModMenuModule::Utils::ToggleNativeCheat::ToggleNativeCheatMode>& ModMenuModule::Utils::ToggleNativeCheat::GetAllToggleNativeCheatModes()
{
	static std::vector<ToggleNativeCheatMode> modes = {
		ToggleNativeCheatMode::UnchangedEnabled,
		ToggleNativeCheatMode::UnchangedDisabled,
		ToggleNativeCheatMode::DisabledEnabled,
		ToggleNativeCheatMode::Cycle
	};
	return modes;
}

ModMenuModule::NativeCheatState ModMenuModule::Utils::ToggleNativeCheat::GetNextState(NativeCheatState currentState, ToggleNativeCheatMode mode)
{
	switch (mode) {
	case ToggleNativeCheatMode::UnchangedEnabled:
		return currentState != NativeCheatState::ForceEnabled ? NativeCheatState::ForceEnabled : NativeCheatState::Unchanged;
	case ToggleNativeCheatMode::UnchangedDisabled:
		return currentState != NativeCheatState::ForceDisabled ? NativeCheatState::ForceDisabled : NativeCheatState::Unchanged;
	case ToggleNativeCheatMode::DisabledEnabled:
		return currentState != NativeCheatState::ForceEnabled ? NativeCheatState::ForceEnabled : NativeCheatState::ForceDisabled;
	case ToggleNativeCheatMode::Cycle:
		switch (currentState) {
		default:
		case NativeCheatState::ForceDisabled:
			return NativeCheatState::Unchanged;
		case NativeCheatState::Unchanged:
			return NativeCheatState::ForceEnabled;
		case NativeCheatState::ForceEnabled:
			return NativeCheatState::ForceDisabled;
		}
	default:
		return currentState;
	}
}

std::optional<ModMenuModule::NativeCheatState> ModMenuModule::Utils::ToggleNativeCheat::ToggleNativeCheat(size_t cheatIndex, ToggleNativeCheatMode mode)
{
	NativeCheatsKeeperCheat* nativeCheatsKeeper = NativeCheatsKeeperCheat::GetInstance();
	if (!nativeCheatsKeeper->IsEnabled()) {
		nativeCheatsKeeper->SetEnabled(true);
	}

	NativeCheatState currentState = nativeCheatsKeeper->GetCheatState(cheatIndex);
	NativeCheatState nextState = GetNextState(currentState, mode);
	
	if (!nativeCheatsKeeper->SetCheat(cheatIndex, nextState)) {
		return std::nullopt;
	}

	return nextState;
}
