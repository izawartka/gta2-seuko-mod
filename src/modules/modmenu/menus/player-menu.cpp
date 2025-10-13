#include "player-menu.h"
#include "player-pos-menu.h"
#include "../../../converters/cop-value.h"
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
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	ModMenuModule::ModMenuOptions options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Position", options.textSize);

	auto onEditStop = [this]() {
		if (m_visible) m_menuController->SetActive(true);
	};

	Core::Resolver<short> wantedLevelResolver = Core::MakeResolver(
		[]() { return Game::Memory::GetGame(); },
		mem(&Game::Game::players), idx(0),
		mem(&Game::Player::ped),
		mem(&Game::Ped::copValue)
	);

	UiModule::VarTextSelectOptionList<short> wantedLevelOptionList = { 0, 600, 1600, 3000, 5000, 8000, 12000 };
	UiModule::Text* wantedLevelText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_wantedLevelController = uiRoot->AddController<UiModule::VarTextSelectController<short>>(
		wantedLevelText,
		wantedLevelResolver,
		wantedLevelOptionList,
		UiModule::VarTextSelectControllerOptions{ L"Wanted level: #", L"#" }
	);
	m_wantedLevelController->SetEditStopCallback(onEditStop);
	m_wantedLevelController->SetConverter<CopValueConverter>();

	return true;
}

void ModMenuModule::PlayerMenu::Detach()
{
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	uiRoot->RemoveController(m_wantedLevelController);
	DestroyMenu();
}

void ModMenuModule::PlayerMenu::OnShow()
{
	m_wantedLevelController->SetWatching(true);
}

void ModMenuModule::PlayerMenu::OnHide()
{
	m_wantedLevelController->SetWatching(false);
	m_wantedLevelController->SetEditing(false);
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
	case 2: // Wanted level
		m_menuController->SetActive(false);
		m_wantedLevelController->SetEditing(true);
	default:
		break;
	}
}
