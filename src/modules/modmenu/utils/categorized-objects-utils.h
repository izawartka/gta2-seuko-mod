#pragma once
#include "../common.h"

namespace ModMenuModule::Utils::CategorizedObjects {
	enum class ObjectCategory {
		Basic,
		WeaponCollectibles,
		CarWeaponCollectibles,
		Powerups,
		Skidmarks,
		Projectiles,
		Phones,
		Other,
		Invisible,
	};

	enum class ObjectVariant {
		Default,
		Moving,
		Animating,
		Dead,
		Ringing,
		Small,
		Medium,
		Big,
		Huge
	};

	struct ObjectDef {
		std::wstring name;
		ObjectCategory category;
		std::unordered_map<ObjectVariant, Game::OBJECT_TYPE> variants;
	};

	const std::vector<ObjectCategory>& GetAllCategories();
	const std::vector<const ObjectDef*>& GetObjectDefsByCategory(ObjectCategory category);
	const std::pair<const ObjectDef*, ObjectVariant>& GetObjectDefByType(Game::OBJECT_TYPE type);
	std::vector<ObjectVariant> GetVariantsByObjectDef(const ObjectDef& objectDef);
}
