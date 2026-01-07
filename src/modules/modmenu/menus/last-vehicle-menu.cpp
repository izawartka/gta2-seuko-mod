#include "last-vehicle-menu.h"
#include "../root.h"
#include "../cheats/last-car.h"
#include "../../../converters/car-model.h"
#include "../../../converters/car-remap.h"
#include "../../../converters/car-damage.h"
#include "../utils/fix-car-engine-damage.h"
#include "../utils/fix-car-visual-damage.h"
#include "last-vehicle-physics-menu.h"
#include "last-vehicle-save-menu.h"

ModMenuModule::LastVehicleMenu::LastVehicleMenu()
{

}

ModMenuModule::LastVehicleMenu::~LastVehicleMenu()
{

}

bool ModMenuModule::LastVehicleMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Current vehicle#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	auto lastCarResolver = [this]() {
		return this->GetLastCar();
	};

	// go back
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Physics", options.textSize);
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Save vehicle", options.textSize);

	// model
	UiModule::Margin* modelMargin = uiRoot->AddComponent<UiModule::Margin>(vertCont, options.menuControllerOptions.createdSelectableOptions.markerOffsetX, 0);
	UiModule::Text* modelText = uiRoot->AddComponent<UiModule::Text>(modelMargin, L"", options.textSize);
	m_modelController = uiRoot->AddController<UiModule::VarTextController<Game::CAR_MODEL4>>(
		modelText,
		Core::MakeResolver(
			lastCarResolver,
			mem(&Game::Car::carModel)
		),
		UiModule::VarTextControllerOptions{ L"Model: #", L"#" }
	);
	m_modelController->SetConverter<CarModelConverter>();

	// remap
	UiModule::Text* remapText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	const auto& remapOptionList = Game::Utils::GetAvailableCarRemapsWithPalette();
	auto remapController = m_menuController->CreateLatestItemController<UiModule::VarTextSelectController<std::tuple<short, Game::PALETTE_BASE>>>(
		remapText,
		Core::MakeResolver(
			lastCarResolver,
			mem(&Game::Car::sprite),
			tupl(mem(&Game::Sprite::carColor), mem(&Game::Sprite::paletteBase))
		),
		remapOptionList,
		UiModule::VarTextSelectControllerOptions{ L"Remap: #", L"#" }
	);
	remapController->SetConverter<CarRemapConverter>();

	// engine damage
	UiModule::Text* engineDamageText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto engineDamageController = m_menuController->CreateLatestItemController<UiModule::VarTextEditableController<short>>(
		engineDamageText,
		Core::MakeResolver(
			lastCarResolver,
			mem(&Game::Car::carDamage)
		),
		UiModule::VarTextEditableControllerOptions{ L"Engine damage: #", L"%#" }
	);
	engineDamageController->SetConverter<CarDamageConverter>();

	// fix engine damage button
	auto fixEngineDamageText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"Fix engine damage", options.textSize);
	auto fixEngineDamageBtn = m_menuController->CreateLatestItemController<UiModule::ButtonController>(fixEngineDamageText);
	fixEngineDamageBtn->SetCallback(this, &LastVehicleMenu::FixCarEngineDamage);

	// fix visual damage button
	auto fixVisualDamageText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"Fix visual damage", options.textSize);
	auto fixVisualDamageBtn = m_menuController->CreateLatestItemController<UiModule::ButtonController>(fixVisualDamageText);
	fixVisualDamageBtn->SetCallback(this, &LastVehicleMenu::FixCarVisualDamage);

	// turn engine off button
	auto engineOffText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"Turn engine off", options.textSize);
	auto engineOffBtn = m_menuController->CreateLatestItemController<UiModule::ButtonController>(engineOffText);
	engineOffBtn->SetCallback(this, &LastVehicleMenu::TurnEngineOff);

	// explode button
	auto explodeText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"Explode", options.textSize);
	auto explodeBtn = m_menuController->CreateLatestItemController<UiModule::ButtonController>(explodeText);
	explodeBtn->SetCallback(this, &LastVehicleMenu::ExplodeCar);

	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::LastVehicleMenu::Detach()
{
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	uiRoot->RemoveController(m_modelController);
	DestroyMenu();
}

void ModMenuModule::LastVehicleMenu::OnShow()
{
	m_modelController->SetWatching(true);

	AddEventListener<LastCarStateEvent>(&ModMenuModule::LastVehicleMenu::OnLastCarStateChange);
	UpdateLastCarState();
}

void ModMenuModule::LastVehicleMenu::OnHide()
{
	RemoveEventListener<LastCarStateEvent>();

	m_modelController->SetWatching(false);
}

void ModMenuModule::LastVehicleMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	switch (id) {
	case 0: // Go back
		ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
		break;
	case 1: // Invulnerability
		ModMenuModule::MenuManager::GetInstance()->AddMenu<ModMenuModule::LastVehiclePhysicsMenu>();
		break;
	case 2: // Save vehicle
		ModMenuModule::MenuManager::GetInstance()->AddMenu<ModMenuModule::LastVehicleSaveMenu>();
		break;
	default:
		break;
	}
}

void ModMenuModule::LastVehicleMenu::OnLastCarStateChange(ModMenuModule::LastCarStateEvent& event)
{
	UpdateLastCarState();
}

void ModMenuModule::LastVehicleMenu::UpdateLastCarState()
{
	LastCarCheat* lastCarCheat = LastCarCheat::GetInstance();
	if (!lastCarCheat->IsEnabled() || lastCarCheat->GetLastCarState() == LastCarState::NoCar) {
		ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
		return;
	}

	std::wstring stateText = lastCarCheat->GetLastCarStateMenuName();
	SetTitle(L"#" + stateText + L"#");
}

Game::Car* ModMenuModule::LastVehicleMenu::GetLastCar()
{
	LastCarCheat* lastCarCheat = LastCarCheat::GetInstance();
	if (!lastCarCheat->IsEnabled()) {
		return nullptr;
	}
	return lastCarCheat->GetLastCar();
}

void ModMenuModule::LastVehicleMenu::FixCarEngineDamage()
{
	Game::Car* lastCar = GetLastCar();
	if (!lastCar) return;

	Utils::FixCarEngineDamage(lastCar);
}

void ModMenuModule::LastVehicleMenu::FixCarVisualDamage()
{
	Game::Car* lastCar = GetLastCar();
	if (!lastCar) return;

	Utils::FixCarVisualDamage(lastCar);
}

void ModMenuModule::LastVehicleMenu::TurnEngineOff()
{
	Game::Car* lastCar = GetLastCar();
	if (!lastCar) {
		return;
	}

	if (lastCar->engineState != Game::CAR_ENGINE_STATE_ON) {
		spdlog::warn("The engine is already off or broken.");
		return;
	}

	lastCar->engineState = Game::CAR_ENGINE_STATE_TURNING_OFF;
}

void ModMenuModule::LastVehicleMenu::ExplodeCar()
{
	Game::Car* lastCar = GetLastCar();
	if (!lastCar) {
		return;
	}

	Game::Functions::ExplodeCar(lastCar, 0, Game::EXPLOSION_SIZE_MEDIUM);
}
