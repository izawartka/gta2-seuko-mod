#pragma once
#include <string>
#include "../modules/modmenu/utils/categorized-objects-utils.h"

class CategorizedObjectCategoryConverter {
public:
	static std::wstring ConvertToString(ModMenuModule::Utils::CategorizedObjects::ObjectCategory value) {
		using ObjectCategory = ModMenuModule::Utils::CategorizedObjects::ObjectCategory;
		switch (value) {
		case ObjectCategory::Basic: return L"Basic";
		case ObjectCategory::WeaponCollectibles: return L"Weapon collectibles";
		case ObjectCategory::CarWeaponCollectibles: return L"Car weapon collectibles";
		case ObjectCategory::Powerups: return L"Power-ups";
		case ObjectCategory::Skidmarks: return L"Skidmarks";
		case ObjectCategory::Projectiles: return L"Projectiles";
		case ObjectCategory::Phones: return L"Phones";
		case ObjectCategory::Other: return L"Other";
		case ObjectCategory::Invisible: return L"Invisible";
		default: return L"???";
		}
	}

	static bool AreEqual(ModMenuModule::Utils::CategorizedObjects::ObjectCategory a, ModMenuModule::Utils::CategorizedObjects::ObjectCategory b) {
		return a == b;
	}

protected:
	CategorizedObjectCategoryConverter() = delete;
	~CategorizedObjectCategoryConverter() = delete;
	CategorizedObjectCategoryConverter(const CategorizedObjectCategoryConverter&) = delete;
	CategorizedObjectCategoryConverter& operator=(const CategorizedObjectCategoryConverter&) = delete;
};
