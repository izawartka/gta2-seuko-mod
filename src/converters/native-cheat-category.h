#pragma once
#include <string>
#include "../modules/modmenu/utils/categorized-native-cheats-utils.h"

class NativeCheatCategoryConverter {
public:
	static std::wstring ConvertToString(ModMenuModule::CategorizedNativeCheats::NativeCheatCategory value) {
		const auto* categoryDef = ModMenuModule::CategorizedNativeCheats::GetCategoryDef(value);
		if (categoryDef == nullptr) {
			return L"???";
		}
		return categoryDef->name;
	}

	static bool AreEqual(
		ModMenuModule::CategorizedNativeCheats::NativeCheatCategory a, 
		ModMenuModule::CategorizedNativeCheats::NativeCheatCategory b
	) {
		return a == b;
	}

protected:
	NativeCheatCategoryConverter() = delete;
	~NativeCheatCategoryConverter() = delete;
	NativeCheatCategoryConverter(const NativeCheatCategoryConverter&) = delete;
	NativeCheatCategoryConverter& operator=(const NativeCheatCategoryConverter&) = delete;
};
