#include "categorized-objects-utils.h"
#include "categorized-objects-data.cpp"

const std::vector<ModMenuModule::Utils::CategorizedObjects::ObjectCategory>& ModMenuModule::Utils::CategorizedObjects::GetAllCategories()
{
	return categories;
}

const std::vector<const ModMenuModule::Utils::CategorizedObjects::ObjectDef*>& ModMenuModule::Utils::CategorizedObjects::GetObjectDefsByCategory(ObjectCategory category)
{
	return objectDefsByCategoryMap.at(category);
}

const std::pair<
	const ModMenuModule::Utils::CategorizedObjects::ObjectDef*,
	ModMenuModule::Utils::CategorizedObjects::ObjectVariant
>& ModMenuModule::Utils::CategorizedObjects::GetObjectDefByType(Game::OBJECT_TYPE type)
{
	return objectTypeToDefMap.at(type);
}

std::vector<ModMenuModule::Utils::CategorizedObjects::ObjectVariant> ModMenuModule::Utils::CategorizedObjects::GetVariantsByObjectDef(const ObjectDef& objectDef)
{
	std::vector<ObjectVariant> variants;
	variants.reserve(objectDef.variants.size());
	for (const auto& pair : objectDef.variants) {
		variants.push_back(pair.first);
	}
	return variants;
}
