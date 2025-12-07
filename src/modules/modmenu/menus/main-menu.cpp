#include "main-menu.h"
#include "../root.h"
#include "player-menu.h"
#include "vehicles-menu.h"
#include "weapons-menu.h"
#include "camera-menu.h"
#include "world-menu.h"
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
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

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
	ModMenuModule::MenuManager* menuManager = ModMenuModule::MenuManager::GetInstance();

	switch (id) {
	case 0: // Player
		menuManager->AddMenu<ModMenuModule::PlayerMenu>();
		break;
	case 1: // Vehicles
		menuManager->AddMenu<ModMenuModule::VehiclesMenu>();
		break;
	case 2: // Weapons
		menuManager->AddMenu<ModMenuModule::WeaponsMenu>();
		break;
	case 3: // Camera
		menuManager->AddMenu<ModMenuModule::CameraMenu>();
		break;
	case 4: // World
		menuManager->AddMenu<ModMenuModule::WorldMenu>();
		break;
	case 5: // Native Cheats
		menuManager->AddMenu<ModMenuModule::NativeCheatsMenu>();
		break;
	case 6: // Misc
		menuManager->AddMenu<ModMenuModule::MiscMenu>();
		break;
	default:
		break;
	}
}
