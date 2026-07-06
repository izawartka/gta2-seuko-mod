#include "spawn-object-segment.h"
#include "../../../converters/categorized-object-category.h"
#include "../../../converters/categorized-object-def.h"
#include "../../../converters/categorized-object-variant.h"
#include "../root.h"

ModMenuModule::SpawnObjectSegment::SpawnObjectSegment(std::string_view persistencePrefix)
{
	m_persistencePrefix = persistencePrefix;
}

ModMenuModule::SpawnObjectSegment::~SpawnObjectSegment()
{

}

std::optional<ModMenuModule::SpawnObjectSegmentData> ModMenuModule::SpawnObjectSegment::GetSegmentData() const
{
	if (!m_categoryController || !m_objectController) {
		spdlog::error("Cannot get segment data: controllers are not initialized.");
		return std::nullopt;
	}

	const Utils::CategorizedObjects::ObjectDef* objectDef = m_objectController->GetValue().value();
	Utils::CategorizedObjects::ObjectVariant variant = GetSelectedVariant();

	Game::OBJECT_TYPE objectType = objectDef->variants.at(variant);

	return SpawnObjectSegmentData{
		objectType
	};
}

bool ModMenuModule::SpawnObjectSegment::SetSegmentData(const SpawnObjectSegmentData& data)
{
	if (!m_categoryController) {
		spdlog::error("Cannot set segment data: category controller is not initialized.");
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
	UpdateSpritePreview();

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
	m_variantCont = uiRoot->AddComponent<UiModule::VertCont>(m_vertCont);
	m_variantItemIndex = m_menuController->GetNextAddedItemIndex();
	if (variantOptionList.size() > 1) {
		CreateVariantController(variantOptionList, selectedVariant);
	}

	// sprite preview
	m_spritePreviewMargin = uiRoot->AddComponent<UiModule::Margin>(
		m_vertCont,
		options.menuControllerOptions.createdSelectableOptions.markerOffsetX,
		options.menuSpacerHeight
	);
	UpdateSpritePreview();

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
	m_variantCont = nullptr;
	m_spritePreview = nullptr;
	m_spritePreviewMargin = nullptr;
	DestroySegment();
}

void ModMenuModule::SpawnObjectSegment::UpdateSpritePreview()
{
	if (!m_spritePreviewMargin) return;

	if (!m_objectController) {
		DestroySpritePreview();
		return;
	}

	const Utils::CategorizedObjects::ObjectDef* objectDef = m_objectController->GetValue().value();
	Utils::CategorizedObjects::ObjectVariant variant = GetSelectedVariant();
	Game::OBJECT_TYPE objectType = objectDef->variants.at(variant);

	size_t objectTypeIndex = static_cast<size_t>(objectType);
	if (objectTypeIndex < 1 || objectTypeIndex >= 300) {
		spdlog::warn("SpawnObjectSegment: Invalid object type {} for sprite preview", objectTypeIndex);
		DestroySpritePreview();
		return;
	}

	Game::ObjectPrefabsContainer* prefabsContainer = Game::Memory::GetObjectPrefabsContainer();
	Game::ObjectPrefab* prefab = prefabsContainer->objectPrefabArr[static_cast<size_t>(objectType)];
	if (!prefab) {
		spdlog::warn("SpawnObjectSegment: No prefab found for object type {}", objectTypeIndex);
		DestroySpritePreview();
		return;
	}

	uint16_t spriteId = prefab->defaultSpriteId;
	Game::SPRITE_TYPE spriteType = prefab->defaultSpriteType;

	if (spriteType == Game::SPRITE_TYPE_INVISIBLE) {
		DestroySpritePreview();
		return;
	}

	UiModule::SpriteOptions options = {};
	options.spriteType = prefab->defaultSpriteType;
	options.spriteId = prefab->defaultSpriteId;
	options.palette = prefab->paletteBase;
	options.remap = prefab->remap;
	options.scale = SPAWN_OBJECT_SPRITE_PREVIEW_SCALE;
	options.rotation = 0.0f;

	if (!m_spritePreview) {
		UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
		m_spritePreview = uiRoot->AddComponent<UiModule::Sprite>(m_spritePreviewMargin, options);
	}
	else {
		m_spritePreview->SetOptions(options);
	}
}

void ModMenuModule::SpawnObjectSegment::DestroySpritePreview()
{
	if (!m_spritePreview) return;

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	uiRoot->RemoveComponent(m_spritePreview, true);
	m_spritePreview = nullptr;
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
	if (m_variantItemIndex == -1 || m_variantController) return;

	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	UiModule::MenuItemGroupId prevGroupId = m_menuController->GetCurrentGroupId();
	m_menuController->SetCurrentGroupId(m_menuGroupId);

	m_menuController->SetNextAddedItemIndex(m_variantItemIndex);
	auto* variantText = m_menuController->CreateItem<UiModule::Text>(m_variantCont, L"", options.textSize);
	m_variantMenuItemId = m_menuController->GetLatestMenuItemId();
	m_variantController = m_menuController->CreateLatestItemController<UiModule::SelectController<Utils::CategorizedObjects::ObjectVariant>>(
		variantText,
		variantOptionList,
		selectedVariant,
		UiModule::SelectControllerOptions{ L"Variant: #", L"#" }
	);
	m_variantController->SetConverter<CategorizedObjectVariantConverter>();
	m_variantController->SetSaveCallback(std::bind(&ModMenuModule::SpawnObjectSegment::OnVariantControllerSave, this));

	m_menuController->SetCurrentGroupId(prevGroupId);
}

void ModMenuModule::SpawnObjectSegment::DestroyVariantController()
{
	if (!m_variantController) return;
	m_menuController->DeleteItem(m_variantMenuItemId);
	m_variantController = nullptr;
	m_variantMenuItemId = -1;
}

void ModMenuModule::SpawnObjectSegment::OnObjectControllerSave(const Utils::CategorizedObjects::ObjectDef* objectDef)
{
	DestroyVariantController();
	auto variantOptionList = Utils::CategorizedObjects::GetVariantsByObjectDef(*objectDef);
	if (variantOptionList.size() > 1) {
		auto selectedVariant = variantOptionList[0];
		CreateVariantController(variantOptionList, selectedVariant);
	}
	UpdateSpritePreview();
}

void ModMenuModule::SpawnObjectSegment::OnVariantControllerSave()
{
	UpdateSpritePreview();
}

ModMenuModule::Utils::CategorizedObjects::ObjectVariant ModMenuModule::SpawnObjectSegment::GetSelectedVariant() const
{
	if (m_variantController && m_variantController->GetValue().has_value()) {
		return m_variantController->GetValue().value();
	}

	if (!m_objectController || !m_objectController->GetValue().has_value()) {
		return Utils::CategorizedObjects::ObjectVariant::Default;
	}

	const Utils::CategorizedObjects::ObjectDef* objectDef = m_objectController->GetValue().value();
	auto variantOptionList = Utils::CategorizedObjects::GetVariantsByObjectDef(*objectDef);

	if (variantOptionList.size() == 1) {
		return variantOptionList[0];
	}

	return Utils::CategorizedObjects::ObjectVariant::Default;
}