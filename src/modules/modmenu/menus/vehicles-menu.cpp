#include "vehicles-menu.h"
#include "spawn-vehicle-menu.h"
#include "../root.h"

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
	ModMenuModule::ModMenuOptions options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Spawn vehicle", options.textSize);


	ApplyIndexPersistence("ModMenu_VehiclesMenu_SelectedIndex");

	return true;
}

void ModMenuModule::VehiclesMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	switch (id) {
	case 0: // Go back
		ModMenuModule::RootModule::GetInstance()->RemoveLastMenu();
		break;
	case 1: // Spawn vehicle
		ModMenuModule::RootModule::GetInstance()->AddMenu<ModMenuModule::SpawnVehicleMenu>();
		break;
	default:
		break;
	}
}
