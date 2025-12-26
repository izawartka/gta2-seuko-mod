#pragma once
#include <string>
#include "../modules/modmenu/cheats/native-cheats-keeper.h"

class NativeCheatConverter {
public:
	static std::wstring ConvertToString(size_t value) {
		const ModMenuModule::NativeCheatDef* cheatDef = ModMenuModule::NativeCheatsKeeperCheat::GetNativeCheatDef(value);
		if (cheatDef) {
			return cheatDef->name;
		}
		return L"Cheat " + std::to_wstring(value);
	}

	static bool AreEqual(size_t a, size_t b) {
		return a == b;
	}

protected:
	NativeCheatConverter() = delete;
	~NativeCheatConverter() = delete;
	NativeCheatConverter(const NativeCheatConverter&) = delete;
	NativeCheatConverter& operator=(const NativeCheatConverter&) = delete;
};
