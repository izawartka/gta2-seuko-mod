#include "camera-menu.h"
#include "camera-pos-menu.h"
#include "camera-advanced-menu.h"
#include "../root.h"

ModMenuModule::CameraMenu::CameraMenu()
{

}

ModMenuModule::CameraMenu::~CameraMenu()
{

}

bool ModMenuModule::CameraMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Camera#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Camera position", options.textSize);
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Advanced camera options", options.textSize);
	
	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::CameraMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	switch (id) {
	case 0: // Go back
		ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
		break;
	case 1: // Camera position
		ModMenuModule::MenuManager::GetInstance()->AddMenu<CameraPosMenu>();
		break;
	case 2: // Advanced camera settings
		ModMenuModule::MenuManager::GetInstance()->AddMenu<CameraAdvancedMenu>();
		break;
	default:
		break;
	}
}
