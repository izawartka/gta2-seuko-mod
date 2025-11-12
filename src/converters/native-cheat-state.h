#pragma once
#include "../modules/modmenu/cheats/native-cheats-keeper.h"
#include <string>

template <bool showEnabledDisabled>
class NativeCheatStateConverter {
public:
	static std::wstring ConvertToString(ModMenuModule::NativeCheatState value) {
		switch (value) {
		case ModMenuModule::NativeCheatState::ForceDisabled:
			return showEnabledDisabled ? L"Disabled" : L"";
		case ModMenuModule::NativeCheatState::Unchanged:
			return L"Unchanged";
		case ModMenuModule::NativeCheatState::ForceEnabled:
			return showEnabledDisabled ? L"Enabled" : L"";
		default:
			return L"Unknown";
		}
	}

	static bool AreEqual(ModMenuModule::NativeCheatState a, ModMenuModule::NativeCheatState b) {
		return a == b;
	}

protected:
	NativeCheatStateConverter() = delete;
	~NativeCheatStateConverter() = delete;
	NativeCheatStateConverter(const NativeCheatStateConverter&) = delete;
	NativeCheatStateConverter& operator=(const NativeCheatStateConverter&) = delete;
};
