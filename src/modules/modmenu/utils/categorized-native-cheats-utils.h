#pragma once
#include "../common.h"

namespace ModMenuModule::Utils::CategorizedNativeCheats {
	enum class NativeCheatCategory {
		Standard,
		SkipThings,
		DebugInfo,
		TilesRelated,
		RequiringRestart,
		ReplayRelated,
		UnstableOrUnused,
		Logging,
		OtherDebug
	};

	struct NativeCheatCategoryDef {
		NativeCheatCategory category;
		std::wstring name;
	};

	struct NativeCheatDef {
		NativeCheatCategory category;
		std::wstring name;
		size_t index;
	};

	const std::vector<NativeCheatCategory>& GetAllCategories();
	const std::vector<NativeCheatCategoryDef>& GetAllCategoryDefs();
	const std::vector<NativeCheatDef>& GetAllCheatDefs();
	const std::vector<const NativeCheatDef*>& GetCheatDefsByCategory(NativeCheatCategory category);
	const std::vector<size_t>& GetCheatsByCategory(NativeCheatCategory category);
	const NativeCheatCategoryDef* GetCategoryDef(NativeCheatCategory category);
	const NativeCheatDef* GetCheatDef(size_t index);
}
