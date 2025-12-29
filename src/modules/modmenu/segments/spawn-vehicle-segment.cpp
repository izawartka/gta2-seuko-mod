#include "spawn-vehicle-segment.h"
#include "../../../converters/car-model.h"
#include "../../../converters/car-remap.h"
#include "../root.h"


ModMenuModule::SpawnVehicleSegment::SpawnVehicleSegment(const std::string& persistencePrefix)
{
	m_persistencePrefix = persistencePrefix;
}

ModMenuModule::SpawnVehicleSegment::~SpawnVehicleSegment()
{

}

std::optional<ModMenuModule::SpawnVehicleSegmentData> ModMenuModule::SpawnVehicleSegment::GetSegmentData() const
{
	if (!m_modelController || !m_remapController) {
		spdlog::error("Cannot get segment data: controllers are not initialized.");
		return std::nullopt;
	}

	return SpawnVehicleSegmentData{
		m_modelController->GetValue().value(),
		std::get<0>(m_remapController->GetValue().value()),
		std::get<1>(m_remapController->GetValue().value())
	};
}

bool ModMenuModule::SpawnVehicleSegment::SetSegmentData(const SpawnVehicleSegmentData& data)
{
	if (!m_modelController || !m_remapController) {
		spdlog::error("Cannot set segment data: controllers are not initialized.");
		return false;
	}

	m_modelController->SetValue(data.model);
	m_remapController->SetValue(std::make_tuple(data.remap, data.palette));
	return true;
}

bool ModMenuModule::SpawnVehicleSegment::Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent)
{
	CreateSegment(menu, parent);

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();

	const auto& modelOptionList = Game::Utils::GetSpawnableCarModels();
	auto selectedModel = modelOptionList[0];
	const auto& remapOptionList = Game::Utils::GetAvailableCarRemapsWithPalette();
	auto selectedRemap = remapOptionList[0];

	if(m_persistencePrefix.size()) {
		selectedModel = persistence->Load(m_persistencePrefix + "_SelectedModel", modelOptionList[0]);
		selectedRemap = persistence->Load(m_persistencePrefix + "_SelectedRemap", remapOptionList[0]);
	}

	// model
	UiModule::Text* modelText = m_menuController->CreateItem<UiModule::Text>(m_vertCont, L"", options.textSize);
	m_modelController = m_menuController->CreateLatestItemController<UiModule::SelectController<Game::CAR_MODEL4>>(
		modelText,
		modelOptionList,
		selectedModel,
		UiModule::SelectControllerOptions{ L"Model: #", L"#" }
	);
	m_modelController->SetConverter<CarModelConverter>();
	m_modelController->SetSaveCallback([this](Game::CAR_MODEL4 newModel) {
		UpdateSpritePreview();
	});

	// remap
	UiModule::Text* remapText = m_menuController->CreateItem<UiModule::Text>(m_vertCont, L"", options.textSize);
	m_remapController = m_menuController->CreateLatestItemController<UiModule::SelectController<std::tuple<short, Game::PALETTE_BASE>>>(
		remapText,
		remapOptionList,
		selectedRemap,
		UiModule::SelectControllerOptions{ L"Remap: #", L"#" }
	);
	m_remapController->SetConverter<CarRemapConverter>();
	m_remapController->SetSaveCallback([this](auto) {
		UpdateSpritePreview();
	});

	// sprite preview
	UpdateSpritePreview();
	auto* spritePreviewMargin = uiRoot->AddComponent<UiModule::Margin>(
		m_vertCont, 
		options.menuControllerOptions.createdSelectableOptions.markerOffsetX, 
		options.menuSpacerHeight
	);
	m_spritePreview = uiRoot->AddComponent<UiModule::CarSprite>(spritePreviewMargin, m_spritePreviewOptions);

	return true;
}

void ModMenuModule::SpawnVehicleSegment::Detach()
{
	if (m_persistencePrefix.size()) {
		PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();
		persistence->Save(m_persistencePrefix + "_SelectedModel", m_modelController->GetValue().value());
		persistence->Save(m_persistencePrefix + "_SelectedRemap", m_remapController->GetValue().value());
	}

	DestroySegment();
	m_spritePreview = nullptr; // has to be cleared because is used in UpdateSpritePreview to check if it exists
}

void ModMenuModule::SpawnVehicleSegment::UpdateSpritePreview()
{
	Game::CAR_MODEL4 selectedModel = m_modelController->GetValue().value();
	auto selectedRemapTuple = m_remapController->GetValue().value();

	UiModule::CarSpriteOptions options = {};
	options.carModel = selectedModel;
	options.rotation = (float)M_PI / 2.0f;
	options.scale = SPAWN_VEHICLE_SPRITE_PREVIEW_SCALE;
	options.remap = std::get<0>(selectedRemapTuple);
	options.palette = std::get<1>(selectedRemapTuple);

	if (m_spritePreview) {
		m_spritePreview->SetOptions(options);
	}

	m_spritePreviewOptions = options;
}
