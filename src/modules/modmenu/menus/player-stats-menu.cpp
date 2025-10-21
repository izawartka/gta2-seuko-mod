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

	Core::Resolver<Game::Player> playerResolver = Core::MakeResolver(
		Game::Memory::GetGame,
		mem(&Game::Game::CurrentPlayer)
	);

	auto onEditStop = [this]() {
		if (m_visible) m_menuController->SetActive(true);
	};

	// money
	UiModule::Text* moneyText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_moneyController = uiRoot->AddController<UiModule::VarTextEditableController<Game::uint>>(
		moneyText,
		Core::MakeResolver(
			playerResolver,
			mem(&Game::Player::animatedMoney),
			mem(&Game::SaveSlotAnimatedValue::value)
		),
		UiModule::VarTextEditableControllerOptions{ L"Money: #$", L"#" }
	);
	m_moneyController->SetEditStopCallback(onEditStop);

	// multiplier
	UiModule::Text* multiplierText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_multiplierController = uiRoot->AddController<UiModule::VarTextEditableController<Game::uint>>(
		multiplierText,
		Core::MakeResolver(
			playerResolver,
			mem(&Game::Player::moneyMultiplier),
			mem(&Game::SaveSlotAnimatedValue::value)
		),
		UiModule::VarTextEditableControllerOptions{ L"Multiplier: #", L"#" }
	);
	m_multiplierController->SetEditStopCallback(onEditStop);

	// lives
	UiModule::Text* livesText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_livesController = uiRoot->AddController<UiModule::VarTextEditableController<Game::uint>>(
		livesText,
		Core::MakeResolver(
			playerResolver,
			mem(&Game::Player::lives),
			mem(&Game::SaveSlotAnimatedValue::value)
		),
		UiModule::VarTextEditableControllerOptions{ L"Lives: #", L"#" }
	);
	m_livesController->SetEditStopCallback(onEditStop);

	auto respect1Resolver = Core::MakeResolver(
		Game::Memory::GetGangRespectContainer,
		mem(&Game::GangRespectContainer::gang), idx(0),
		mem(&Game::GangRespect::respect)
	);

	spdlog::debug("Respect 1 address: {}", fmt::ptr(respect1Resolver()));

	// respect 1
	UiModule::Text* respect1Text = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_respect1Controller = uiRoot->AddController<UiModule::VarTextEditableController<char>>(
		respect1Text,
		Core::MakeResolver(
			Game::Memory::GetGangRespectContainer,
			mem(&Game::GangRespectContainer::gang), idx(0),
			mem(&Game::GangRespect::respect)
		),
		UiModule::VarTextEditableControllerOptions{ L"Respect 1: #", L"#" }
	);
	m_respect1Controller->SetEditStopCallback(onEditStop);

	// respect 2
	UiModule::Text* respect2Text = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_respect2Controller = uiRoot->AddController<UiModule::VarTextEditableController<char>>(
		respect2Text,
		Core::MakeResolver(
			Game::Memory::GetGangRespectContainer,
			mem(&Game::GangRespectContainer::gang), idx(1),
			mem(&Game::GangRespect::respect)
		),
		UiModule::VarTextEditableControllerOptions{ L"Respect 2: #", L"#" }
	);
	m_respect2Controller->SetEditStopCallback(onEditStop);

	// respect 3
	UiModule::Text* respect3Text = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_respect3Controller = uiRoot->AddController<UiModule::VarTextEditableController<char>>(
		respect3Text,
		Core::MakeResolver(
			Game::Memory::GetGangRespectContainer,
			mem(&Game::GangRespectContainer::gang), idx(2),
			mem(&Game::GangRespect::respect)
		),
		UiModule::VarTextEditableControllerOptions{ L"Respect 3: #", L"#" }
	);
	m_respect3Controller->SetEditStopCallback(onEditStop);

	ApplyIndexPersistence("ModMenu_PlayerStatsMenu_SelectedIndex");

	return true;
}

void ModMenuModule::PlayerStatsMenu::Detach()
{
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	uiRoot->RemoveController(m_moneyController);
	uiRoot->RemoveController(m_multiplierController);
	uiRoot->RemoveController(m_livesController);
	uiRoot->RemoveController(m_respect1Controller);
	uiRoot->RemoveController(m_respect2Controller);
	uiRoot->RemoveController(m_respect3Controller);
	DestroyMenu();
}

void ModMenuModule::PlayerStatsMenu::OnShow()
{
	m_moneyController->SetWatching(true);
	m_multiplierController->SetWatching(true);
	m_livesController->SetWatching(true);
	m_respect1Controller->SetWatching(true);
	m_respect2Controller->SetWatching(true);
	m_respect3Controller->SetWatching(true);
}

void ModMenuModule::PlayerStatsMenu::OnHide()
{
	m_moneyController->SetWatching(false);
	m_moneyController->SetEditing(false);
	m_multiplierController->SetWatching(false);
	m_multiplierController->SetEditing(false);
	m_livesController->SetWatching(false);
	m_livesController->SetEditing(false);
	m_respect1Controller->SetWatching(false);
	m_respect1Controller->SetEditing(false);
	m_respect2Controller->SetWatching(false);
	m_respect2Controller->SetEditing(false);
	m_respect3Controller->SetWatching(false);
	m_respect3Controller->SetEditing(false);
}

void ModMenuModule::PlayerStatsMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	switch (id) {
	case 0: // Go back
		ModMenuModule::RootModule::GetInstance()->RemoveLastMenu();
		break;
	case 1: // Money
		m_menuController->SetActive(false);
		m_moneyController->SetEditing(true);
		break;
	case 2: // Multiplier
		m_menuController->SetActive(false);
		m_multiplierController->SetEditing(true);
		break;
	case 3: // Lives
		m_menuController->SetActive(false);
		m_livesController->SetEditing(true);
		break;
	case 4: // Respect 1
		m_menuController->SetActive(false);
		m_respect1Controller->SetEditing(true);
		break;
	case 5: // Respect 2
		m_menuController->SetActive(false);
		m_respect2Controller->SetEditing(true);
		break;
	case 6: // Respect 3
		m_menuController->SetActive(false);
		m_respect3Controller->SetEditing(true);
		break;
	default:
		break;
	}
}
