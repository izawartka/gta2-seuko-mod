#include "player-stats-menu.h"
#include "../root.h"

ModMenuModule::PlayerStatsMenu::PlayerStatsMenu()
{

}

ModMenuModule::PlayerStatsMenu::~PlayerStatsMenu()
{

}

bool ModMenuModule::PlayerStatsMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Player stats#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	ModMenuModule::ModMenuOptions options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);

	// money
	UiModule::Text* moneyText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_menuController->CreateLatestItemController<UiModule::VarTextEditableController<Game::uint>>(
		moneyText,
		Core::MakeResolver(
			Game::Memory::GetGame,
			mem(&Game::Game::CurrentPlayer),
			mem(&Game::Player::animatedMoney),
			mem(&Game::SaveSlotAnimatedValue::value)
		),
		UiModule::VarTextEditableControllerOptions{ L"Money: #$", L"#" }
	);

	// multiplier
	UiModule::Text* multiplierText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_menuController->CreateLatestItemController<UiModule::VarTextEditableController<Game::uint>>(
		multiplierText,
		Core::MakeResolver(
			Game::Memory::GetGame,
			mem(&Game::Game::CurrentPlayer),
			mem(&Game::Player::moneyMultiplier),
			mem(&Game::SaveSlotAnimatedValue::value)
		),
		UiModule::VarTextEditableControllerOptions{ L"Multiplier: #", L"#" }
	);

	// lives
	UiModule::Text* livesText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_menuController->CreateLatestItemController<UiModule::VarTextEditableController<Game::uint>>(
		livesText,
		Core::MakeResolver(
			Game::Memory::GetGame,
			mem(&Game::Game::CurrentPlayer),
			mem(&Game::Player::lives),
			mem(&Game::SaveSlotAnimatedValue::value)
		),
		UiModule::VarTextEditableControllerOptions{ L"Lives: #", L"#" }
	);

	// respect 1
	UiModule::Text* respect1Text = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_menuController->CreateLatestItemController<UiModule::VarTextEditableController<char>>(
		respect1Text,
		Core::MakeResolver(
			Game::Memory::GetGangRespectContainer,
			mem(&Game::GangRespectContainer::gang), idx(0),
			mem(&Game::GangRespect::respect)
		),
		UiModule::VarTextEditableControllerOptions{ L"Respect 1: #", L"#" }
	);

	// respect 2
	UiModule::Text* respect2Text = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_menuController->CreateLatestItemController<UiModule::VarTextEditableController<char>>(
		respect2Text,
		Core::MakeResolver(
			Game::Memory::GetGangRespectContainer,
			mem(&Game::GangRespectContainer::gang), idx(1),
			mem(&Game::GangRespect::respect)
		),
		UiModule::VarTextEditableControllerOptions{ L"Respect 2: #", L"#" }
	);

	// respect 3
	UiModule::Text* respect3Text = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_menuController->CreateLatestItemController<UiModule::VarTextEditableController<char>>(
		respect3Text,
		Core::MakeResolver(
			Game::Memory::GetGangRespectContainer,
			mem(&Game::GangRespectContainer::gang), idx(2),
			mem(&Game::GangRespect::respect)
		),
		UiModule::VarTextEditableControllerOptions{ L"Respect 3: #", L"#" }
	);

	ApplyIndexPersistence("ModMenu_PlayerStatsMenu_SelectedIndex");

	return true;
}

void ModMenuModule::PlayerStatsMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	switch (id) {
	case 0: // Go back
		ModMenuModule::RootModule::GetInstance()->RemoveLastMenu();
		break;
	default:
		break;
	}
}
