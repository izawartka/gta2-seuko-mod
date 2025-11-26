#include "spawn-vehicle-segment.h"
#include "../../../converters/car-model.h"
#include "../../../converters/yes-no.h"
#include "../../../converters/car-remap.h"
#include "../root.h"


ModMenuModule::SpawnVehicleSegment::SpawnVehicleSegment(const std::string& persistencePrefix)
{
	m_persistencePrefix = persistencePrefix;
}

ModMenuModule::SpawnVehicleSegment::~SpawnVehicleSegment()
{

}

bool ModMenuModule::SpawnVehicleSegment::Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent)
{
	CreateSegment(menu, parent);

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();

	UiModule::SelectOptionList<Game::CAR_MODEL4> modelOptionList = Game::Utils::GetSpawnableCarModels();
	Game::CAR_MODEL4 selectedModel = modelOptionList[0];
	auto remapOptionList = Game::Utils::GetAvailableCarRemapsWithPalette();
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
		UpdateSpritePreviewMargin();
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
	m_spritePreviewMargin = uiRoot->AddComponent<UiModule::Margin>(m_vertCont, 0, 0);
	m_spritePreview = uiRoot->AddComponent<UiModule::Sprite>(m_spritePreviewMargin, m_spritePreviewOptions);
	UpdateSpritePreviewMargin();

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

std::optional<ModMenuModule::SpawnVehicleSegmentData> ModMenuModule::SpawnVehicleSegment::GetSegmentData() const
{
	if(!m_modelController || !m_remapController) {
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
	if(!m_modelController || !m_remapController) {
		spdlog::error("Cannot set segment data: controllers are not initialized.");
		return false;
	}

	m_modelController->SetValue(data.model);
	m_remapController->SetValue(std::make_tuple(data.remap, data.palette));
	return true;
}

void ModMenuModule::SpawnVehicleSegment::UpdateSpritePreview()
{
	Game::CAR_MODEL4 selectedModel = m_modelController->GetValue().value();
	auto selectedRemapTuple = m_remapController->GetValue().value();

	UiModule::SpriteOptions options = {};
	options.rotation = (float)M_PI / 2.0f;
	options.scale = SPAWN_VEHICLE_SPRITE_PREVIEW_SCALE;
	options.spriteType = Game::SPRITE_TYPE::SPRITE_TYPE_CAR;
	options.remap = std::get<0>(selectedRemapTuple);
	options.palette = std::get<1>(selectedRemapTuple);

	Game::Style_S3* styleS3 = Game::Memory::GetStyleS3();
	Game::CarInfo* carInfo = styleS3->allCarsInfo->cars[selectedModel];
	if (!carInfo) {
		spdlog::error("Could not find vehicle info for the model #{}.", static_cast<uint32_t>(selectedModel));
		return;
	}
	uint16_t spriteId = static_cast<uint16_t>(carInfo->sprite);
	options.spriteId = spriteId;

	if (m_spritePreview) {
		m_spritePreview->SetOptions(options);
	}

	m_spritePreviewOptions = options;
}

void ModMenuModule::SpawnVehicleSegment::UpdateSpritePreviewMargin()
{
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	Game::CAR_MODEL4 selectedModel = m_modelController->GetValue().value();

	Game::Style_S3* styleS3 = Game::Memory::GetStyleS3();
	Game::CarInfo* carInfo = styleS3->allCarsInfo->cars[selectedModel];
	if (!carInfo) {
		spdlog::error("Could not find vehicle info for the model #{}.", static_cast<uint32_t>(selectedModel));
		return;
	}

	uint16_t carWidth = carInfo->w;
	uint16_t carHeight = carInfo->h;
	int spriteWidth = m_spritePreview->GetSpriteWidth();
	int spriteHeight = m_spritePreview->GetSpriteHeight();
	float offsetX = (float)(spriteHeight - carHeight) / 2.0f;
	if (offsetX < 0) offsetX = 0;
	float offsetY = (float)(spriteWidth - carWidth) / 2.0f;
	if (offsetY < 0) offsetY = 0;

	Game::SCR_f scaledOffsetX = Game::Utils::Multiply(Game::Utils::FromFloat(offsetX), SPAWN_VEHICLE_SPRITE_PREVIEW_SCALE);
	Game::SCR_f marginX = options.menuControllerOptions.createdSelectableOptions.markerOffsetX - scaledOffsetX;
	Game::SCR_f scaledOffsetY = Game::Utils::Multiply(Game::Utils::FromFloat(offsetY), SPAWN_VEHICLE_SPRITE_PREVIEW_SCALE);
	Game::SCR_f marginY = options.menuSpacerHeight * 2 - scaledOffsetY;

	m_spritePreviewMargin->SetMargin(marginX, marginY);
}
