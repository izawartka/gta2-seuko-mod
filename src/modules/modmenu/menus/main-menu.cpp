#include "main-menu.h"
#include "../root.h"
#include "player-menu.h"
#include "vehicles-menu.h"

ModMenuModule::MainMenu::MainMenu()
{

}

ModMenuModule::MainMenu::~MainMenu()
{

}

bool ModMenuModule::MainMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Mod Menu#", vertCont);
	ModMenuModule::ModMenuOptions options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Player", options.textSize);
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Vehicles", options.textSize);
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Weapons", options.textSize);
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Camera", options.textSize);
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"World", options.textSize);
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Misc", options.textSize);

	ApplyIndexPersistence("ModMenu_MainMenu_SelectedIndex");

	return true;
}

void ModMenuModule::MainMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	switch (id) {
	case 0: // Player
		ModMenuModule::RootModule::GetInstance()->AddMenu<ModMenuModule::PlayerMenu>();
		break;
	case 1: // Vehicles
		ModMenuModule::RootModule::GetInstance()->AddMenu<ModMenuModule::VehiclesMenu>();
		break;
	default:
		break;
	}
}
