#include "player-menu.h"
#include "player-pos-menu.h"
#include "../root.h"

ModMenuModule::PlayerMenu::PlayerMenu()
{

}

ModMenuModule::PlayerMenu::~PlayerMenu()
{

}

bool ModMenuModule::PlayerMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Player#", vertCont);
	ModMenuModule::ModMenuOptions options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Position", options.textSize);

	return true;
}

void ModMenuModule::PlayerMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	switch (id) {
	case 0: // Go back
		ModMenuModule::RootModule::GetInstance()->RemoveLastMenu();
		break;
	case 1: // Position
		ModMenuModule::RootModule::GetInstance()->AddMenu<ModMenuModule::PlayerPosMenu>();
		break;
	default:
		break;
	}
}
