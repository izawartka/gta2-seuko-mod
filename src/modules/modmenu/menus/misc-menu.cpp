#include "misc-menu.h"
#include "../root.h"
#include "menu-controls-menu.h"
#include "quick-actions-menu.h"
#include "version-menu.h"
#include "../utils/save-game.h"
#include "../toast-manager.h"

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
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Menu controls", options.textSize);
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Quick actions", options.textSize);
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Version details", options.textSize);

	// quick save button
	auto quickSaveText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"Quick save", options.textSize);
	auto quickSaveBtn = m_menuController->CreateLatestItemController<UiModule::ButtonController>(quickSaveText);
	quickSaveBtn->SetCallback(this, &MiscMenu::QuickSave);

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
	case 2: // Quick actions
		ModMenuModule::MenuManager::GetInstance()->AddMenu<ModMenuModule::QuickActionsMenu>();
		break;
	case 3: // Version info
		ModMenuModule::MenuManager::GetInstance()->AddMenu<ModMenuModule::VersionMenu>();
		break;
	default:
		break;
	}
}

void ModMenuModule::MiscMenu::QuickSave()
{
	if (ModMenuModule::Utils::SaveGame()) {
		ModMenuModule::ToastManager::GetInstance()->Show({ L"Game saved" });
	}
	else {
		ModMenuModule::ToastManager::GetInstance()->Show({ L"Failed to save game", ToastType::Error });
	}
}
