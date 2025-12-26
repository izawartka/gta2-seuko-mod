#pragma once
#include <string>
#include "../modules/modmenu/utils/toggle-native-cheat-utils.h"

class ToggleNativeCheatModeConverter {
public:
	static std::wstring ConvertToString(ModMenuModule::Utils::ToggleNativeCheat::ToggleNativeCheatMode value) {

		switch (value) {
		case ModMenuModule::Utils::ToggleNativeCheat::ToggleNativeCheatMode::UnchangedEnabled:
			return L"Unchanged - Enabled";
		case ModMenuModule::Utils::ToggleNativeCheat::ToggleNativeCheatMode::UnchangedDisabled:
			return L"Unchanged - Disabled";
		case ModMenuModule::Utils::ToggleNativeCheat::ToggleNativeCheatMode::DisabledEnabled:
			return L"Disabled - Enabled";
		case ModMenuModule::Utils::ToggleNativeCheat::ToggleNativeCheatMode::Cycle:
			return L"Cycle";
		default: return L"???";
		}
	}

	static bool AreEqual(
		ModMenuModule::Utils::ToggleNativeCheat::ToggleNativeCheatMode a, 
		ModMenuModule::Utils::ToggleNativeCheat::ToggleNativeCheatMode b
	) {
		return a == b;
	}

protected:
	ToggleNativeCheatModeConverter() = delete;
	~ToggleNativeCheatModeConverter() = delete;
	ToggleNativeCheatModeConverter(const ToggleNativeCheatModeConverter&) = delete;
	ToggleNativeCheatModeConverter& operator=(const ToggleNativeCheatModeConverter&) = delete;
};
