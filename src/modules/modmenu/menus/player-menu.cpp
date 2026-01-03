#include "player-menu.h"
#include "player-pos-menu.h"
#include "player-stats-menu.h"
#include "player-appearance-menu.h"
#include "../../../converters/cop-value.h"
#include "../../../converters/yes-no.h"
#include "../cheats/cop-value.h"
#include "../cheats/invulnerability.h"
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
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Position", options.textSize);
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Stats", options.textSize);
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Appearance", options.textSize);

	CopValueCheat* copValueCheat = CopValueCheat::GetInstance();
	if (!copValueCheat->IsEnabled()) {
		copValueCheat->SetEnabled(true);
	}

	// wanted level
	UiModule::VarTextSelectOptionList<short> wantedLevelOptionList = { 0, 600, 1600, 3000, 5000, 8000, 12000 };
	UiModule::Text* wantedLevelText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto wantedLevelController = m_menuController->CreateLatestItemController<UiModule::VarTextSelectController<short, short>>(
		wantedLevelText,
		[copValueCheat]() {
			return copValueCheat->GetCopValue().value_or(0);
		},
		wantedLevelOptionList,
		UiModule::VarTextSelectControllerOptions{ L"Wanted level: #", L"#" }
	);
	wantedLevelController->SetConverter<CopValueConverter>();
	wantedLevelController->SetCustomSaveCallback([copValueCheat](short newWantedLevel) {
		copValueCheat->SetCopValue(newWantedLevel);
	});

	// freeze wanted level
	UiModule::Text* freezeCopValueText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto freezeCopValueController = m_menuController->CreateLatestItemController<UiModule::VarTextSelectController<bool, bool>>(
		freezeCopValueText,
		[copValueCheat]() {
			return copValueCheat->IsCopValueLocked();
		},
		UiModule::VarTextSelectOptionList<bool>{ false, true },
		UiModule::VarTextSelectControllerOptions{ L"Freeze wanted level: #", L"#" }
	);
	freezeCopValueController->SetConverter<YesNoConverter>();
	freezeCopValueController->SetCustomSaveCallback([copValueCheat](bool newValue) {
		copValueCheat->SetCopValueLocked(newValue);
	});

	// health
	UiModule::Text* healthText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto healthController = m_menuController->CreateLatestItemController<UiModule::VarTextEditableController<Game::ushort>>(
		healthText,
		Core::MakeResolver(
			Game::Memory::GetPlayerPed,
			mem(&Game::Ped::health)
		),
		UiModule::VarTextEditableControllerOptions{ L"Health: #", L"#" }
	);

	// armor
	UiModule::Text* armorText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto armorController = m_menuController->CreateLatestItemController<UiModule::VarTextEditableController<Game::ushort>>(
		armorText,
		Core::MakeResolver(
			Game::Memory::GetGame,
			mem(&Game::Game::CurrentPlayer),
			mem(&Game::Player::armor)
		),
		UiModule::VarTextEditableControllerOptions{ L"Armor: #", L"#" }
	);

	// invulnerability
	UiModule::Text* invulnerabilityText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_invulnerabilityController = m_menuController->CreateLatestItemController<UiModule::SelectController<bool>>(
		invulnerabilityText,
		UiModule::SelectOptionList<bool>{ false, true },
		std::nullopt,
		UiModule::SelectControllerOptions{ L"Invulnerability: #", L"#" }
	);
	m_invulnerabilityController->SetConverter<YesNoConverter>();
	m_invulnerabilityController->SetSaveCallback([](bool newValue) {
		InvulnerabilityCheat::GetInstance()->SetEnabled(newValue);
	});

	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::PlayerMenu::OnShow()
{
	AddEventListener<ModMenuModule::CheatStateEvent>(&PlayerMenu::OnCheatStateChange);
	UpdateCheatStates();
}

void ModMenuModule::PlayerMenu::OnHide()
{
	RemoveEventListener<ModMenuModule::CheatStateEvent>();
}

void ModMenuModule::PlayerMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	switch (id) {
	case 0: // Go back
		ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
		break;
	case 1: // Position
		ModMenuModule::MenuManager::GetInstance()->AddMenu<ModMenuModule::PlayerPosMenu>();
		break;
	case 2: // Stats
		ModMenuModule::MenuManager::GetInstance()->AddMenu<ModMenuModule::PlayerStatsMenu>();
		break;
	case 3: // Appearance
		ModMenuModule::MenuManager::GetInstance()->AddMenu<ModMenuModule::PlayerAppearanceMenu>();
		break;
	default:
		break;
	}
}

void ModMenuModule::PlayerMenu::OnCheatStateChange(CheatStateEvent& event)
{
	if (event.GetCheatType() == typeid(InvulnerabilityCheat)) {
		m_invulnerabilityController->SetValue(event.IsEnabled());
	}
}

void ModMenuModule::PlayerMenu::UpdateCheatStates()
{
	m_invulnerabilityController->SetValue(InvulnerabilityCheat::GetInstance()->IsEnabled());
}
