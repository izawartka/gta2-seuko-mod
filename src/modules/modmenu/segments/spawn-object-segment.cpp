#include "spawn-object-segment.h"
#include "../../../converters/categorized-object-category.h"
#include "../../../converters/categorized-object-def.h"
#include "../../../converters/categorized-object-variant.h"
#include "../root.h"

ModMenuModule::SpawnObjectSegment::SpawnObjectSegment(const std::string& persistencePrefix)
{
	m_persistencePrefix = persistencePrefix;
}

ModMenuModule::SpawnObjectSegment::~SpawnObjectSegment()
{

}

std::optional<ModMenuModule::SpawnObjectSegmentData> ModMenuModule::SpawnObjectSegment::GetSegmentData() const
{
	if (!m_categoryController || !m_objectController || !m_variantController) {
		spdlog::error("Cannot get segment data: controllers are not initialized.");
		return std::nullopt;
	}

	const Utils::CategorizedObjects::ObjectDef* objectDef = m_objectController->GetValue().value();
	Utils::CategorizedObjects::ObjectVariant variant = m_variantController->GetValue().value();

	Game::OBJECT_TYPE objectType = objectDef->variants.at(variant);

	return SpawnObjectSegmentData{
		objectType
	};
}

bool ModMenuModule::SpawnObjectSegment::SetSegmentData(const SpawnObjectSegmentData& data)
{
	if (!m_categoryController || !m_objectController || !m_variantController) {
		spdlog::error("Cannot set segment data: controllers are not initialized.");
		return false;
	}

	auto& pair = Utils::CategorizedObjects::GetObjectDefByType(data.objectType);
	const Utils::CategorizedObjects::ObjectDef* objectDef = pair.first;
	Utils::CategorizedObjects::ObjectVariant variant = pair.second;

	m_categoryController->SetValue(objectDef->category);
	DestroyObjectController();
	const auto& objectOptionList = Utils::CategorizedObjects::GetObjectDefsByCategory(objectDef->category);
	CreateObjectController(objectOptionList, objectDef);
	m_objectController->SetValue(objectDef);
	DestroyVariantController();
	const auto& variantOptionList = Utils::CategorizedObjects::GetVariantsByObjectDef(*objectDef);
	CreateVariantController(variantOptionList, variant);
	m_variantController->SetValue(variant);

	return true;
}

bool ModMenuModule::SpawnObjectSegment::Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent)
{
	CreateSegment(menu, parent);

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();

	const std::pair<const Utils::CategorizedObjects::ObjectDef*, Utils::CategorizedObjects::ObjectVariant>* persistenceData = nullptr;
	if (m_persistencePrefix.size()) {
		auto selectedObjectType = persistence->LoadOptional<Game::OBJECT_TYPE>(m_persistencePrefix + "_SelectedObjectType");
		if (selectedObjectType.has_value()) {
			persistenceData = &Utils::CategorizedObjects::GetObjectDefByType(*selectedObjectType);
		}
	}

	const auto& categoryOptionList = Utils::CategorizedObjects::GetAllCategories();
	auto selectedCategory = persistenceData ? persistenceData->first->category : categoryOptionList[0];

	const auto& objectOptionList = Utils::CategorizedObjects::GetObjectDefsByCategory(selectedCategory);
	const auto* selectedObject = persistenceData ? persistenceData->first : objectOptionList[0];

	auto variantOptionList = Utils::CategorizedObjects::GetVariantsByObjectDef(*selectedObject);
	auto selectedVariant = persistenceData ? persistenceData->second : variantOptionList[0];

	// category
	UiModule::Text* categoryText = m_menuController->CreateItem<UiModule::Text>(m_vertCont, L"", options.textSize);
	m_categoryController = m_menuController->CreateLatestItemController<UiModule::SelectController<Utils::CategorizedObjects::ObjectCategory>>(
		categoryText,
		categoryOptionList,
		selectedCategory,
		UiModule::SelectControllerOptions{L"Category: #", L"#"}
	);
	m_categoryController->SetConverter<CategorizedObjectCategoryConverter>();
	m_categoryController->SetSaveCallback(std::bind(&ModMenuModule::SpawnObjectSegment::OnCategoryControllerSave, this, std::placeholders::_1));

	// object
	m_objectText = m_menuController->CreateItem<UiModule::Text>(m_vertCont, L"", options.textSize);
	m_objectMenuItemId = m_menuController->GetLatestMenuItemId();
	CreateObjectController(objectOptionList, selectedObject);

	// variant
	m_variantText = m_menuController->CreateItem<UiModule::Text>(m_vertCont, L"", options.textSize);
	m_variantMenuItemId = m_menuController->GetLatestMenuItemId();
	CreateVariantController(variantOptionList, selectedVariant);

	return true;
}

void ModMenuModule::SpawnObjectSegment::Detach()
{
	if (m_persistencePrefix.size()) {
		PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();
		auto segmentData = GetSegmentData();
		if (segmentData.has_value()) {
			persistence->Save<Game::OBJECT_TYPE>(m_persistencePrefix + "_SelectedObjectType", segmentData.value().objectType);
		}
	}

	m_objectMenuItemId = -1;
	m_objectController = nullptr;
	m_variantMenuItemId = -1;
	m_variantController = nullptr;
	DestroySegment();
}

void ModMenuModule::SpawnObjectSegment::CreateObjectController(const std::vector<const Utils::CategorizedObjects::ObjectDef*>& objectOptionList, const Utils::CategorizedObjects::ObjectDef* selectedObject)
{
	if (!m_objectText || m_objectMenuItemId == -1 || m_objectController) return;

	m_objectController = m_menuController->CreateItemController<UiModule::SelectController<const Utils::CategorizedObjects::ObjectDef*>>(
		m_objectMenuItemId,
		m_objectText,
		objectOptionList,
		selectedObject,
		UiModule::SelectControllerOptions{ L"Object: #", L"#" }
	);
	m_objectController->SetConverter<CategorizedObjectDefConverter>();
	m_objectController->SetSaveCallback(std::bind(&ModMenuModule::SpawnObjectSegment::OnObjectControllerSave, this, std::placeholders::_1));
}

void ModMenuModule::SpawnObjectSegment::DestroyObjectController()
{
	if (!m_objectController || m_objectMenuItemId == -1) return;
	m_menuController->DeleteItemController(m_objectMenuItemId);
	m_objectController = nullptr;
}

void ModMenuModule::SpawnObjectSegment::OnCategoryControllerSave(Utils::CategorizedObjects::ObjectCategory category)
{
	DestroyObjectController();
	const auto& objectOptionList = Utils::CategorizedObjects::GetObjectDefsByCategory(category);
	const auto* selectedObject = objectOptionList[0];
	CreateObjectController(objectOptionList, selectedObject);
	OnObjectControllerSave(selectedObject);
}

void ModMenuModule::SpawnObjectSegment::CreateVariantController(const std::vector<Utils::CategorizedObjects::ObjectVariant>& variantOptionList, Utils::CategorizedObjects::ObjectVariant selectedVariant)
{
	if (!m_variantText || m_variantMenuItemId == -1 || m_variantController) return;

	m_variantController = m_menuController->CreateItemController<UiModule::SelectController<Utils::CategorizedObjects::ObjectVariant>>(
		m_variantMenuItemId,
		m_variantText,
		variantOptionList,
		selectedVariant,
		UiModule::SelectControllerOptions{ L"Variant: #", L"#" }
	);
	m_variantController->SetConverter<CategorizedObjectVariantConverter>();
}

void ModMenuModule::SpawnObjectSegment::DestroyVariantController()
{
	if (!m_variantController || m_variantMenuItemId == -1) return;
	m_menuController->DeleteItemController(m_variantMenuItemId);
	m_variantController = nullptr;
}

void ModMenuModule::SpawnObjectSegment::OnObjectControllerSave(const Utils::CategorizedObjects::ObjectDef* objectDef)
{
	DestroyVariantController();
	auto variantOptionList = Utils::CategorizedObjects::GetVariantsByObjectDef(*objectDef);
	auto selectedVariant = variantOptionList[0];
	CreateVariantController(variantOptionList, selectedVariant);
}
