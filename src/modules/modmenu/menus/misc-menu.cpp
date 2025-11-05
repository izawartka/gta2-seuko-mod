#include "misc-menu.h"
#include "../root.h"
#include "menu-controls-menu.h"

ModMenuModule::MiscMenu::MiscMenu()
{

}

ModMenuModule::MiscMenu::~MiscMenu()
{

}

bool ModMenuModule::MiscMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Misc#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	ModMenuModule::ModMenuOptions options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Menu controls", options.textSize);

	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::MiscMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	switch (id) {
	case 0: // Go back
		ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
		break;
	case 1: // Menu controls
		ModMenuModule::MenuManager::GetInstance()->AddMenu<ModMenuModule::MenuControlsMenu>();
		break;
	default:
		break;
	}
}
