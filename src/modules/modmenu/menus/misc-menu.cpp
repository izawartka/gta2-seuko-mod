#include "misc-menu.h"
#include "../root.h"
#include "quick-actions-menu.h"
#include "seukomod-options-menu.h"
#include "../utils/save-game.h"
#include "../utils/unlock-all-levels.h"
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
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Quick actions", options.textSize);
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Seuko mod options", options.textSize);

	// quick save button
	auto quickSaveText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"Quick save", options.textSize);
	auto quickSaveBtn = m_menuController->CreateLatestItemController<UiModule::ButtonController>(quickSaveText);
	quickSaveBtn->SetCallback(this, &MiscMenu::QuickSave);

	// unlock all levels button
	auto unlockAllLevelsText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"Unlock all levels", options.textSize);
	auto unlockAllLevelsBtn = m_menuController->CreateLatestItemController<UiModule::ButtonController>(unlockAllLevelsText);
	unlockAllLevelsBtn->SetCallback(this, &MiscMenu::UnlockAllLevels);

	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::MiscMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	switch (id) {
	case 0: // Go back
		ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
		break;
	case 1: // Quick actions
		ModMenuModule::MenuManager::GetInstance()->AddMenu<ModMenuModule::QuickActionsMenu>();
		break;
	case 2: // Seuko mod options
		ModMenuModule::MenuManager::GetInstance()->AddMenu<ModMenuModule::SeukomodOptionsMenu>();
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

void ModMenuModule::MiscMenu::UnlockAllLevels()
{
	if (ModMenuModule::Utils::UnlockAllLevels()) {
		ModMenuModule::ToastManager::GetInstance()->Show({ L"All levels unlocked" });
	}
	else {
		ModMenuModule::ToastManager::GetInstance()->Show({ L"Failed to unlock all levels", ToastType::Error });
	}
}
