#include "player-stats-menu.h"
#include "../root.h"
#include "../segments/gang-respect-segment.h"

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
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);

	// money
	UiModule::Text* moneyText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_menuController->CreateLatestItemController<UiModule::VarTextEditableController<Game::uint>>(
		moneyText,
		Core::MakeResolver(
			Game::Memory::GetGame,
			mem(&Game::Game::currentPlayer),
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
			mem(&Game::Game::currentPlayer),
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
			mem(&Game::Game::currentPlayer),
			mem(&Game::Player::lives),
			mem(&Game::SaveSlotAnimatedValue::value)
		),
		UiModule::VarTextEditableControllerOptions{ L"Lives: #", L"#" }
	);

	std::vector<Game::GangRespect*> gangs = Game::Utils::GetVisibleGangs();
	for (Game::GangRespect* gang : gangs) {
		CreateAttachSegment<GangRespectSegment>(this, vertCont, gang);
	}

	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::PlayerStatsMenu::Detach()
{
	ClearSegments();
	DestroyMenu();
}

void ModMenuModule::PlayerStatsMenu::OnShow()
{
	SetSegmentsVisible(true);
}

void ModMenuModule::PlayerStatsMenu::OnHide()
{
	SetSegmentsVisible(false);
}

void ModMenuModule::PlayerStatsMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	switch (id) {
	case 0: // Go back
		ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
		break;
	default:
		break;
	}
}
