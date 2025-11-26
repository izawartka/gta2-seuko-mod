#include "vehicles-menu.h"
#include "spawn-vehicle-menu.h"
#include "last-vehicle-menu.h"
#include "../root.h"
#include "../cheats/last-car.h"

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

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Spawn vehicle", options.textSize);
	m_lastCarText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"No last vehicle", options.textSize);

	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::VehiclesMenu::OnShow()
{
	AddEventListener<LastCarStateEvent>(&ModMenuModule::VehiclesMenu::OnLastCarStateChange);
	UpdateLastCarState();
}

void ModMenuModule::VehiclesMenu::OnHide()
{
	RemoveEventListener<LastCarStateEvent>();
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
		lastCarCheat = GetCheat<LastCarCheat>();
		if (!lastCarCheat || lastCarCheat->GetLastCarState() == LastCarState::NoCar) {
			spdlog::warn("No last vehicle to show in Last Vehicle menu");
			return;
		}
		ModMenuModule::MenuManager::GetInstance()->AddMenu<ModMenuModule::LastVehicleMenu>();
		break;
	default:
		break;
	}
}

void ModMenuModule::VehiclesMenu::OnLastCarStateChange(ModMenuModule::LastCarStateEvent& event)
{
	UpdateLastCarState();
}

void ModMenuModule::VehiclesMenu::UpdateLastCarState()
{
	if (!m_lastCarText) return;

	LastCarCheat* lastCarCheat = GetCheat<LastCarCheat>();
	if (!lastCarCheat) return;

	std::wstring stateText = lastCarCheat->GetLastCarStateMenuName();
	m_lastCarText->SetText(stateText);
}
