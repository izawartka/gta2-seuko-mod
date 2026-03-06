#include "categorized-native-cheats-utils.h"
#include "categorized-native-cheats-data.cpp"

namespace ModMenuModule::CategorizedNativeCheats {
	const std::vector<NativeCheatCategory>& GetAllCategories()
	{
		return nativeCheatCategories;
	}

	const std::vector<NativeCheatCategoryDef>& GetAllCategoryDefs()
	{
		return nativeCheatCategoryDefs;
	}

	const std::vector<NativeCheatDef>& GetAllCheatDefs()
	{
		return nativeCheatDefs;
	}

	const std::vector<const NativeCheatDef*>& GetCheatDefsByCategory(NativeCheatCategory category)
	{
		return nativeCheatDefsByCategoryMap.at(category);
	}

	const std::vector<size_t>& GetCheatsByCategory(NativeCheatCategory category)
	{
		return nativeCheatsByCategoryMap.at(category);
	}

	const NativeCheatCategoryDef* GetCategoryDef(NativeCheatCategory category)
	{
		auto it = nativeCheatCategoryDefMap.find(category);
		if (it != nativeCheatCategoryDefMap.end()) {
			return it->second;
		}
		return nullptr;
	}

	const NativeCheatDef* GetCheatDef(size_t index)
	{
		auto it = nativeCheatDefMap.find(index);
		if (it != nativeCheatDefMap.end()) {
			return it->second;
		}
		return nullptr;
	}
}
