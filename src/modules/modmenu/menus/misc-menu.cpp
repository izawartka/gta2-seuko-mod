#include "misc-menu.h"
#include "../root.h"
#include "menu-controls-menu.h"
#include "quick-actions-menu.h"

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
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Quick actions", options.textSize);

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
	default:
		break;
	}
}

void ModMenuModule::MiscMenu::QuickSave()
{
	Game::Ped* playerPed = Game::Memory::GetPlayerPed();
	if (!playerPed) {
		spdlog::warn("Cannot save: Player ped not found");
		return;
	}

	Game::S15_Script* s15 = Game::Memory::GetS15();
	if (s15 == nullptr) {
		spdlog::warn("Cannot save: S15 script not found");
		return;
	}

	bool* missionFlagPtr = (bool*)s15->missionPtrMaybe;
	if (missionFlagPtr == nullptr) {
		spdlog::warn("Cannot save: Mission flag pointer not found");
		return;
	}

	if (*missionFlagPtr != 0) {
		spdlog::warn("Cannot save during a mission");
		return;
	}

	playerPed->y -= Game::Utils::FromFloat(1.0f);

	Game::Menu* menu = Game::Memory::GetMenu();
	Game::Functions::SaveGame(s15, 0, menu->saveFile);

	playerPed->y += Game::Utils::FromFloat(1.0f);
	spdlog::info("Game quick saved");
}
