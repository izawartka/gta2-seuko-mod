#include "spawn-vehicle-menu.h"
#include "../../../converters/car-model.h"
#include "../../../converters/yes-no.h"
#include "../../../converters/car-remap.h"
#include "../root.h"

ModMenuModule::SpawnVehicleMenu::SpawnVehicleMenu()
{

}

ModMenuModule::SpawnVehicleMenu::~SpawnVehicleMenu()
{

}

bool ModMenuModule::SpawnVehicleMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Spawn vehicle#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	ModMenuModule::ModMenuOptions options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	ModMenuModule::PersistenceManager* persistence = ModMenuModule::PersistenceManager::GetInstance();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);

	// model
	UiModule::Text* modelText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	UiModule::VarTextSelectOptionList<Game::CAR_MODEL4> modelOptionList = Game::Utils::GetSpawnableCarModels();
	m_selectedModel = persistence->Load("ModMenu_SpawnVehicleMenu_SelectedModel", modelOptionList[0]);
	m_modelController = uiRoot->AddController<UiModule::VarTextSelectController<Game::CAR_MODEL4>>(
		modelText,
		[this]() { return &m_selectedModel; },
		modelOptionList,
		UiModule::VarTextSelectControllerOptions{ L"Model: #", L"#" }
	);
	m_modelController->SetEditStopCallback([this]() {
		if (m_visible) m_menuController->SetActive(true);
	});
	m_modelController->SetConverter<CarModelConverter>();
	m_modelController->SetCustomSaveCallback([this](Game::CAR_MODEL4 newModel) {
		m_selectedModel = newModel;
		UpdateSpritePreview();
		UpdateSpritePreviewMargin();
	});

	// remap
	UiModule::Text* remapText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	UiModule::VarTextSelectOptionList<short> remapOptionList = Game::Utils::GetAvailableCarRemaps();
	m_selectedRemap = persistence->Load("ModMenu_SpawnVehicleMenu_SelectedRemap", remapOptionList[0]);
	m_remapController = uiRoot->AddController<UiModule::VarTextSelectController<short>>(
		remapText,
		[this]() { return &m_selectedRemap; },
		remapOptionList,
		UiModule::VarTextSelectControllerOptions{ L"Remap: #", L"#" }
	);
	m_remapController->SetEditStopCallback([this]() {
		if (m_visible) m_menuController->SetActive(true);
	});
	m_remapController->SetConverter<CarRemapConverter>();
	m_remapController->SetCustomSaveCallback([this](short newRemap) {
		m_selectedRemap = newRemap;
		UpdateSpritePreview();
	});

	// sprite preview
	UpdateSpritePreview();
	m_spritePreviewMargin = uiRoot->AddComponent<UiModule::Margin>(vertCont, 0, 0);
	m_spritePreview = uiRoot->AddComponent<UiModule::Sprite>(m_spritePreviewMargin, m_spritePreviewOptions);
	UpdateSpritePreviewMargin();

	// spawn button
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Spawn", options.textSize);
	ApplyIndexPersistence("ModMenu_SpawnVehicleMenu_SelectedIndex");

	return true;
}

void ModMenuModule::SpawnVehicleMenu::Detach()
{
	ModMenuModule::PersistenceManager* persistence = ModMenuModule::PersistenceManager::GetInstance();
	persistence->Save("ModMenu_SpawnVehicleMenu_SelectedModel", m_selectedModel);
	persistence->Save("ModMenu_SpawnVehicleMenu_SelectedRemap", m_selectedRemap);

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	uiRoot->RemoveController(m_modelController);
	uiRoot->RemoveController(m_remapController);

	DestroyMenu();
}

void ModMenuModule::SpawnVehicleMenu::OnShow()
{
	m_modelController->SetWatching(true);
	m_remapController->SetWatching(true);
}

void ModMenuModule::SpawnVehicleMenu::OnHide()
{
	m_modelController->SetWatching(false);
	m_modelController->SetEditing(false);
	m_remapController->SetWatching(false);
	m_remapController->SetEditing(false);
}

void ModMenuModule::SpawnVehicleMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	switch (id) {
	case 0: // Go back
		ModMenuModule::RootModule::GetInstance()->RemoveLastMenu();
		break;
	case 1: // Model
		m_menuController->SetActive(false);
		m_modelController->SetEditing(true);
		break;
	case 2: // Remap
		m_menuController->SetActive(false);
		m_remapController->SetEditing(true);
		break;
	case 3: // Spawn
		Spawn();
		break;
	default:
		break;
	}
}

void ModMenuModule::SpawnVehicleMenu::UpdateSpritePreview() 
{
	UiModule::SpriteOptions options = {};
	options.rotation = M_PI / 2.0f;
	options.scale = SPRITE_PREVIEW_SCALE;
	options.spriteType = Game::SPRITE_TYPE::SPRITE_TYPE_CAR;

	if (m_selectedRemap == -1) {
		options.palette = Game::PALETTE_BASE::PALETTE_BASE_SPRITE;
		options.remap = 0;
	}
	else {
		options.palette = Game::PALETTE_BASE::PALETTE_BASE_CAR_REMAP;
		options.remap = static_cast<uint16_t>(m_selectedRemap);
	}

	Game::Style_S3* styleS3 = Game::Memory::GetStyleS3();
	Game::CarInfo* carInfo = styleS3->allCarsInfo->cars[m_selectedModel];
	if (!carInfo) {
		spdlog::error("Could not find vehicle info for the model #{}.", static_cast<uint32_t>(m_selectedModel));
		return;
	}
	uint16_t spriteId = static_cast<uint16_t>(carInfo->sprite);
	options.spriteId = spriteId;

	if(m_spritePreview) {
		m_spritePreview->SetOptions(options);
	}

	m_spritePreviewOptions = options;
}

void ModMenuModule::SpawnVehicleMenu::UpdateSpritePreviewMargin()
{
	Game::Style_S3* styleS3 = Game::Memory::GetStyleS3();
	Game::CarInfo* carInfo = styleS3->allCarsInfo->cars[m_selectedModel];
	if (!carInfo) {
		spdlog::error("Could not find vehicle info for the model #{}.", static_cast<uint32_t>(m_selectedModel));
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

	Game::SCR_f scaledOffsetX = Game::Utils::Multiply(Game::Utils::FromFloat(offsetX), SPRITE_PREVIEW_SCALE);
	Game::SCR_f marginX = SPRITE_PREVIEW_MARGIN_X - scaledOffsetX;
	Game::SCR_f scaledOffsetY = Game::Utils::Multiply(Game::Utils::FromFloat(offsetY), SPRITE_PREVIEW_SCALE);
	Game::SCR_f marginY = SPRITE_PREVIEW_MARGIN_Y - scaledOffsetY;

	m_spritePreviewMargin->SetMargin(marginX, marginY);
}

void ModMenuModule::SpawnVehicleMenu::Spawn()
{
	Game::Ped* ped = Game::Memory::GetPlayerPed();
	if (!ped || !ped->gameObject || !ped->gameObject->sprite) {
		spdlog::warn("Could not find player ped position to spawn vehicle.");
		return;
	}

	if(ped->currentCar) {
		spdlog::warn("Player is already in a car, cannot spawn another.");
		return;
	}

	Game::Car* car = Game::Functions::SpawnCar(
		ped->gameObject->sprite->x,
		ped->gameObject->sprite->y,
		ped->gameObject->sprite->z,
		ped->gameObject->spriteRotation,
		m_selectedModel
	);

	if(!car) {
		spdlog::error("Failed to spawn vehicle model #{}.", static_cast<uint32_t>(m_selectedModel));
		return;
	}

	if (m_selectedRemap == -1) {
		car->sprite->carColor = 0;
		car->sprite->paletteBase = Game::PALETTE_BASE::PALETTE_BASE_SPRITE;
	}
	else {
		car->sprite->carColor = m_selectedRemap;
		car->sprite->paletteBase = Game::PALETTE_BASE::PALETTE_BASE_CAR_REMAP;
	}

	spdlog::debug("Spawned vehicle model #{} with remap #{}.", static_cast<uint32_t>(m_selectedModel), m_selectedRemap);

	/// TODO: Move player into the car
}
