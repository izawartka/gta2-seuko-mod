#pragma once
#include <string>
#include "../modules/modmenu/utils/categorized-native-cheats-utils.h"

class NativeCheatCategoryConverter {
public:
	static std::wstring ConvertToString(ModMenuModule::Utils::CategorizedNativeCheats::NativeCheatCategory value) {
		const auto* categoryDef = ModMenuModule::Utils::CategorizedNativeCheats::GetCategoryDef(value);
		if (categoryDef == nullptr) {
			return L"???";
		}
		return categoryDef->name;
	}

	static bool AreEqual(
		ModMenuModule::Utils::CategorizedNativeCheats::NativeCheatCategory a, 
		ModMenuModule::Utils::CategorizedNativeCheats::NativeCheatCategory b
	) {
		return a == b;
	}

protected:
	NativeCheatCategoryConverter() = delete;
	~NativeCheatCategoryConverter() = delete;
	NativeCheatCategoryConverter(const NativeCheatCategoryConverter&) = delete;
	NativeCheatCategoryConverter& operator=(const NativeCheatCategoryConverter&) = delete;
};
