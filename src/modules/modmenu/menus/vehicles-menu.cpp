#include "vehicles-menu.h"
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

	return true;
}

void ModMenuModule::VehiclesMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	switch (id) {
	case 0: // Go back
		ModMenuModule::RootModule::GetInstance()->RemoveLastMenu();
		break;
	default:
		break;
	}
}
