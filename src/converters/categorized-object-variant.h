#pragma once
#include <string>
#include "../modules/modmenu/utils/categorized-objects-utils.h"

class CategorizedObjectVariantConverter {
public:
	static std::wstring ConvertToString(ModMenuModule::Utils::CategorizedObjects::ObjectVariant value) {
		using ObjectVariant = ModMenuModule::Utils::CategorizedObjects::ObjectVariant;
		switch (value) {
		case ObjectVariant::Default: return L"Default";
		case ObjectVariant::Moving: return L"Moving";
		case ObjectVariant::Animating: return L"Animating";
		case ObjectVariant::Dead: return L"Dead";
		case ObjectVariant::Ringing: return L"Ringing";
		case ObjectVariant::Small: return L"Small";
		case ObjectVariant::Medium: return L"Medium";
		case ObjectVariant::Big: return L"Big";
		case ObjectVariant::Huge: return L"Huge";
		default: return L"???";
		}
	}

	static bool AreEqual(ModMenuModule::Utils::CategorizedObjects::ObjectVariant a, ModMenuModule::Utils::CategorizedObjects::ObjectVariant b) {
		return a == b;
	}

protected:
	CategorizedObjectVariantConverter() = delete;
	~CategorizedObjectVariantConverter() = delete;
	CategorizedObjectVariantConverter(const CategorizedObjectVariantConverter&) = delete;
	CategorizedObjectVariantConverter& operator=(const CategorizedObjectVariantConverter&) = delete;
};
