#include "player-menu.h"
#include "player-pos-menu.h"
#include "player-stats-menu.h"
#include "../../../converters/cop-value.h"
#include "../../../converters/yes-no.h"
#include "../cheats/freeze-cop-value.h"
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

	// wanted level
	auto wantedLevelResolver = Core::MakeResolver(
		Game::Memory::GetPlayerPed,
		mem(&Game::Ped::copValue)
	);

	UiModule::VarTextSelectOptionList<short> wantedLevelOptionList = { 0, 600, 1600, 3000, 5000, 8000, 12000 };
	UiModule::Text* wantedLevelText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto wantedLevelController = m_menuController->CreateLatestItemController<UiModule::VarTextSelectController<short>>(
		wantedLevelText,
		Core::MakeResolver(
			Game::Memory::GetPlayerPed,
			mem(&Game::Ped::copValue)
		),
		wantedLevelOptionList,
		UiModule::VarTextSelectControllerOptions{ L"Wanted level: #", L"#" }
	);
	wantedLevelController->SetConverter<CopValueConverter>();
	wantedLevelController->SetCustomSaveCallback([wantedLevelResolver](short newWantedLevel) {
		auto* cheat = FreezeCopValueCheat::GetInstance();
		cheat->SetCopValue(newWantedLevel);

		if (!cheat->IsEnabled()) {
			short* copValue = wantedLevelResolver();
			if (!copValue) {
				spdlog::error("Unable to resolve cop value");
				return;
			}

			*copValue = newWantedLevel;
		}

		spdlog::info("Set wanted level to {}", newWantedLevel);
	});

	// freeze wanted level
	UiModule::Text* freezeCopValueText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_freezeCopValueController = m_menuController->CreateLatestItemController<UiModule::SelectController<bool>>(
		freezeCopValueText,
		UiModule::SelectOptionList<bool>{ false, true },
		std::nullopt,
		UiModule::SelectControllerOptions{ L"Freeze wanted level: #", L"#" }
	);
	m_freezeCopValueController->SetConverter<YesNoConverter>();
	m_freezeCopValueController->SetSaveCallback([](bool newValue) {
		FreezeCopValueCheat::GetInstance()->SetEnabled(newValue);
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
	default:
		break;
	}
}

void ModMenuModule::PlayerMenu::OnCheatStateChange(CheatStateEvent& event)
{
	if (event.GetCheatType() == typeid(FreezeCopValueCheat)) {
		m_freezeCopValueController->SetValue(event.IsEnabled());
	}
	else if (event.GetCheatType() == typeid(InvulnerabilityCheat)) {
		m_invulnerabilityController->SetValue(event.IsEnabled());
	}
}

void ModMenuModule::PlayerMenu::UpdateCheatStates()
{
	m_freezeCopValueController->SetValue(FreezeCopValueCheat::GetInstance()->IsEnabled());
	m_invulnerabilityController->SetValue(InvulnerabilityCheat::GetInstance()->IsEnabled());
}
