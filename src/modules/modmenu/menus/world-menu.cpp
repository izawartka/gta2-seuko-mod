#include "world-menu.h"
#include "spawn-object-menu.h"
#include "../root.h"

ModMenuModule::WorldMenu::WorldMenu()
{

}

ModMenuModule::WorldMenu::~WorldMenu()
{

}

bool ModMenuModule::WorldMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#World#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Spawn object", options.textSize);

	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::WorldMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	switch (id) {
	case 0: // Go back
		ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
		break;
	case 1: // Spawn object
		ModMenuModule::MenuManager::GetInstance()->AddMenu<ModMenuModule::SpawnObjectMenu>();
		break;
	default:
		break;
	}
}
