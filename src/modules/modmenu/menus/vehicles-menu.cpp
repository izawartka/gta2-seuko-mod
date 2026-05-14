#include "vehicles-menu.h"
#include "spawn-vehicle-menu.h"
#include "last-vehicle-menu.h"
#include "saved-vehicles-menu.h"
#include "../root.h"
#include "../cheats/last-car.h"
#include "../cheats/disable-steering-assist.h"
#include "../../../converters/yes-no.h"

ModMenuModule::VehiclesMenu::VehiclesMenu()
{

}

ModMenuModule::VehiclesMenu::~VehiclesMenu()
{

}

bool ModMenuModule::VehiclesMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Vehicles#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	DisableSteeringAssistCheat* disableSteeringAssistCheat = DisableSteeringAssistCheat::GetInstance();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Spawn vehicle", options.textSize);
	m_lastCarText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"No last vehicle", options.textSize);
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Saved vehicles", options.textSize);


	// disable steering assist cheat
	UiModule::Text* disableSteeringAssistCheatText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_disableSteeringAssistCheatController = m_menuController->CreateLatestItemController<UiModule::SelectController<bool>>(
		disableSteeringAssistCheatText,
		UiModule::SelectOptionList<bool>{ false, true },
		std::nullopt,
		UiModule::SelectControllerOptions{ L"Disable steering assist: #", L"#" }
	);
	m_disableSteeringAssistCheatController->SetConverter<YesNoConverter>();
	m_disableSteeringAssistCheatController->SetSaveCallback([disableSteeringAssistCheat](bool newValue) {
		disableSteeringAssistCheat->SetEnabled(newValue);
	});

	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::VehiclesMenu::OnShow()
{
	AddEventListener<LastCarStateEvent>(&ModMenuModule::VehiclesMenu::OnLastCarStateChange);
	AddEventListener<CheatStateEvent>(&ModMenuModule::VehiclesMenu::OnCheatStateChange);
	UpdateLastCarState();
	UpdateCheatStates();
}

void ModMenuModule::VehiclesMenu::OnHide()
{
	RemoveEventListener<LastCarStateEvent>();
	RemoveEventListener<CheatStateEvent>();
}

void ModMenuModule::VehiclesMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	LastCarCheat* lastCarCheat;

	switch (id) {
	case 0: // Go back
		ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
		break;
	case 1: // Spawn vehicle
		ModMenuModule::MenuManager::GetInstance()->AddMenu<ModMenuModule::SpawnVehicleMenu>();
		break;
	case 2: // Last vehicle
		lastCarCheat = LastCarCheat::GetInstance();
		if (!lastCarCheat || lastCarCheat->GetLastCarState() == LastCarState::NoCar) {
			spdlog::warn("No last vehicle to show in Last Vehicle menu");
			return;
		}
		ModMenuModule::MenuManager::GetInstance()->AddMenu<ModMenuModule::LastVehicleMenu>();
		break;
	case 3: // Saved vehicles
		ModMenuModule::MenuManager::GetInstance()->AddMenu<ModMenuModule::SavedVehiclesMenu>();
		break;
	default:
		break;
	}
}

void ModMenuModule::VehiclesMenu::OnLastCarStateChange(ModMenuModule::LastCarStateEvent& event)
{
	UpdateLastCarState();
}

void ModMenuModule::VehiclesMenu::OnCheatStateChange(CheatStateEvent& event)
{
	if (event.GetCheatType() == typeid(DisableSteeringAssistCheat)) {
		m_disableSteeringAssistCheatController->SetValue(event.IsEnabled());
	}
}

void ModMenuModule::VehiclesMenu::UpdateLastCarState()
{
	if (!m_lastCarText) return;

	LastCarCheat* lastCarCheat = LastCarCheat::GetInstance();
	if (!lastCarCheat->IsEnabled()) return;

	std::wstring stateText = lastCarCheat->GetLastCarStateMenuName();
	m_lastCarText->SetText(stateText);
}

void ModMenuModule::VehiclesMenu::UpdateCheatStates()
{
	bool disableSteeringAssistCheatEnabled = DisableSteeringAssistCheat::GetInstance()->IsEnabled();
	m_disableSteeringAssistCheatController->SetValue(disableSteeringAssistCheatEnabled);
}
