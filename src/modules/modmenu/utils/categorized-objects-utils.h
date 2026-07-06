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
		Crane,
		Other,
		Invisible,
		UnknownInvisible,
	};

	enum class ObjectVariant {
		Default,
		Moving,
		Animating,
		Dead,
		Ringing,
		Call,
		CallEnd,
		Small,
		Medium,
		Big,
		Huge,
		Loonies,
		Yakuza,
		Zaibatsu,
		Rednecks,
		Scientists,
		Krishna,
		Russian,
		VehicleOnlyMoving,
		Unknown
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
