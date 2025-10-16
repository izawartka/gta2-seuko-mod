#include "player-menu.h"
#include "player-pos-menu.h"
#include "../../../converters/cop-value.h"
#include "../../../converters/yes-no.h"
#include "../cheats/freeze-cop-value.h"
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
		Game::Memory::GetPlayerPed,
		mem(&Game::Ped::copValue)
	);

	UiModule::VarTextSelectOptionList<short> wantedLevelOptionList = { 0, 600, 1600, 3000, 5000, 8000, 12000 };
	UiModule::Text* wantedLevelText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_wantedLevelController = uiRoot->AddController<UiModule::VarTextSelectController<short>>(
		wantedLevelText,
		Core::MakeResolver(
			Game::Memory::GetPlayerPed,
			mem(&Game::Ped::copValue)
		),
		wantedLevelOptionList,
		UiModule::VarTextSelectControllerOptions{ L"Wanted level: #", L"#" }
	);
	m_wantedLevelController->SetEditStopCallback(onEditStop);
	m_wantedLevelController->SetConverter<CopValueConverter>();
	m_wantedLevelController->SetCustomSaveCallback([wantedLevelResolver, this](short newWantedLevel) {
		ModMenuModule::FreezeCopValue* cheat = GetCheat<ModMenuModule::FreezeCopValue>();
		if (cheat) {
			cheat->SetCopValue(newWantedLevel);
		}

		short* copValue = wantedLevelResolver();
		if (copValue) {
			*copValue = newWantedLevel;
		}
	});

	UiModule::Text* freezeCopValueText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_freezeCopValueController = uiRoot->AddController<UiModule::VarTextSelectController<bool>>(
		freezeCopValueText,
		[this]() { return &m_freezeCopValueEnabled; },
		UiModule::VarTextSelectOptionList<bool>{ false, true },
		UiModule::VarTextSelectControllerOptions{ L"Freeze wanted level: #", L"#" }
	);
	m_freezeCopValueController->SetEditStopCallback(onEditStop);
	m_freezeCopValueController->SetConverter<YesNoConverter>();
	m_freezeCopValueController->SetCustomSaveCallback([this](bool newValue) {
		SetCheatEnabled<ModMenuModule::FreezeCopValue>(newValue);
	});

	UiModule::Text* healthText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_healthController = uiRoot->AddController<UiModule::VarTextEditableController<Game::ushort>>(
		healthText,
		Core::MakeResolver(
			Game::Memory::GetPlayerPed,
			mem(&Game::Ped::health)
		),
		UiModule::VarTextEditableControllerOptions{ L"Health: #", L"#" }
	);
	m_healthController->SetEditStopCallback(onEditStop);

	ApplyIndexPersistence("ModMenu_PlayerMenu_SelectedIndex");

	return true;
}

void ModMenuModule::PlayerMenu::Detach()
{
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	uiRoot->RemoveController(m_wantedLevelController);
	uiRoot->RemoveController(m_freezeCopValueController);
	uiRoot->RemoveController(m_healthController);
	DestroyMenu();
}

void ModMenuModule::PlayerMenu::OnShow()
{
	m_wantedLevelController->SetWatching(true);
	m_freezeCopValueController->SetWatching(true);
	m_healthController->SetWatching(true);

	AddEventListener<ModMenuModule::CheatStateEvent>(&PlayerMenu::OnCheatStateChange);
	UpdateCheatStates();
}

void ModMenuModule::PlayerMenu::OnHide()
{
	RemoveEventListener<ModMenuModule::CheatStateEvent>();

	m_wantedLevelController->SetWatching(false);
	m_wantedLevelController->SetEditing(false);
	m_freezeCopValueController->SetWatching(false);
	m_freezeCopValueController->SetEditing(false);
	m_healthController->SetWatching(false);
	m_healthController->SetEditing(false);
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
		break;
	case 3: // Freeze wanted level
		m_menuController->SetActive(false);
		m_freezeCopValueController->SetEditing(true);
		break;
	case 4: // Health
		m_menuController->SetActive(false);
		m_healthController->SetEditing(true);
		break;
	default:
		break;
	}
}

void ModMenuModule::PlayerMenu::OnCheatStateChange(const CheatStateEvent& event)
{
	if (event.GetCheatType() == typeid(ModMenuModule::FreezeCopValue)) {
		m_freezeCopValueEnabled = event.IsEnabled();
	}
}

void ModMenuModule::PlayerMenu::UpdateCheatStates()
{
	ModMenuModule::FreezeCopValue* cheat = GetCheat<ModMenuModule::FreezeCopValue>();
	if (cheat) {
		m_freezeCopValueEnabled = cheat->IsEnabled();
	}
}
