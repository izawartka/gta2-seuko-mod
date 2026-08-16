#include "seukomod-options-menu.h"
#include "../root.h"
#include "menu-controls-menu.h"
#include "version-menu.h"

ModMenuModule::SeukomodOptionsMenu::SeukomodOptionsMenu()
{

}

ModMenuModule::SeukomodOptionsMenu::~SeukomodOptionsMenu()
{

}

bool ModMenuModule::SeukomodOptionsMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Seuko mod options#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Menu controls", options.textSize);
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Version details", options.textSize);

	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::SeukomodOptionsMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	switch (id) {
	case 0: // Go back
		ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
		break;
	case 1: // Menu controls
		ModMenuModule::MenuManager::GetInstance()->AddMenu<ModMenuModule::MenuControlsMenu>();
		break;
	case 2: // Version info
		ModMenuModule::MenuManager::GetInstance()->AddMenu<ModMenuModule::VersionMenu>();
		break;
	default:
		break;
	}
}
