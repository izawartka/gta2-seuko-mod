#pragma once
#include <string>
#include "../modules/modmenu/cheats/native-cheats-keeper.h"

class NativeCheatCategoryConverter {
public:
	static std::wstring ConvertToString(ModMenuModule::NativeCheatCategory value) {
		const ModMenuModule::NativeCheatCategoryDef* categoryDef = ModMenuModule::NativeCheatsKeeperCheat::GetNativeCheatCategoryDef(value);
		if (categoryDef == nullptr) {
			return L"???";
		}
		return categoryDef->name;
	}

	static bool AreEqual(ModMenuModule::NativeCheatCategory a, ModMenuModule::NativeCheatCategory b) {
		return a == b;
	}

protected:
	NativeCheatCategoryConverter() = delete;
	~NativeCheatCategoryConverter() = delete;
	NativeCheatCategoryConverter(const NativeCheatCategoryConverter&) = delete;
	NativeCheatCategoryConverter& operator=(const NativeCheatCategoryConverter&) = delete;
};
