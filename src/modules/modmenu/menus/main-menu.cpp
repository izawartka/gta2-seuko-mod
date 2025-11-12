#include "main-menu.h"
#include "../root.h"
#include "player-menu.h"
#include "vehicles-menu.h"
#include "weapons-menu.h"
#include "native-cheats-menu.h"
#include "misc-menu.h"

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
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Native Cheats", options.textSize);
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Misc", options.textSize);

	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::MainMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	switch (id) {
	case 0: // Player
		ModMenuModule::MenuManager::GetInstance()->AddMenu<ModMenuModule::PlayerMenu>();
		break;
	case 1: // Vehicles
		ModMenuModule::MenuManager::GetInstance()->AddMenu<ModMenuModule::VehiclesMenu>();
		break;
	case 2: // Weapons
		ModMenuModule::MenuManager::GetInstance()->AddMenu<ModMenuModule::WeaponsMenu>();
		break;
	case 3: // Camera
		break;
	case 4: // World
		break;
	case 5: // Native Cheats
		ModMenuModule::MenuManager::GetInstance()->AddMenu<ModMenuModule::NativeCheatsMenu>();
		break;
	case 6: // Misc
		ModMenuModule::MenuManager::GetInstance()->AddMenu<ModMenuModule::MiscMenu>();
		break;
	default:
		break;
	}
}
