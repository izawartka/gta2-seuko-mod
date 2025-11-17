#pragma once
#include <string>
#include "../modules/modmenu/modmenu.h"

class QuickActionTypeConverter {
public:
	static std::wstring ConvertToString(ModMenuModule::QuickActionTypeIndex value) {
		return ModMenuModule::QuickActionManager::GetTypeLabel(value);
	}

	static bool AreEqual(ModMenuModule::QuickActionTypeIndex a, ModMenuModule::QuickActionTypeIndex b) {
		return a == b;
	}

protected:
	QuickActionTypeConverter() = delete;
	~QuickActionTypeConverter() = delete;
	QuickActionTypeConverter(const QuickActionTypeConverter&) = delete;
	QuickActionTypeConverter& operator=(const QuickActionTypeConverter&) = delete;
};
