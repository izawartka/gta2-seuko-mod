#pragma once
#include <string>
#include "../modules/modmenu/utils/categorized-objects-utils.h"

class CategorizedObjectDefConverter {
public:
	static std::wstring ConvertToString(const ModMenuModule::Utils::CategorizedObjects::ObjectDef* value) {
		return value ? value->name : L"???";
	}

	static bool AreEqual(const ModMenuModule::Utils::CategorizedObjects::ObjectDef* a, const ModMenuModule::Utils::CategorizedObjects::ObjectDef* b) {
		return a == b;
	}

protected:
	CategorizedObjectDefConverter() = delete;
	~CategorizedObjectDefConverter() = delete;
	CategorizedObjectDefConverter(const CategorizedObjectDefConverter&) = delete;
	CategorizedObjectDefConverter& operator=(const CategorizedObjectDefConverter&) = delete;
};
