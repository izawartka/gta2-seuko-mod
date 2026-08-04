#include "misc-options-menu.h"
#include "../../root.h"
#include "../../../../converters/ped-state.h"
#include "../../../../converters/ped-state-2.h"
#include "../../../../converters/hex.h"

ModMenuModule::PedTemplateMiscOptionsMenu::PedTemplateMiscOptionsMenu(TempPedTemplateId tempTemplateId)
{
	m_tempTemplateId = tempTemplateId;
}

ModMenuModule::PedTemplateMiscOptionsMenu::~PedTemplateMiscOptionsMenu()
{

}

bool ModMenuModule::PedTemplateMiscOptionsMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Character template options#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);

	uiRoot->AddComponent<UiModule::Spacer>(vertCont, 0, options.menuSpacerHeight);

	PedTemplatesCheat* pedTemplatesCheat = PedTemplatesCheat::GetInstance();
	m_tempTemplatePtr = pedTemplatesCheat->GetTempTemplate(m_tempTemplateId);
	if (!m_tempTemplatePtr) {
		spdlog::error("PedTemplateMiscOptionsMenu: Failed to get temp template with id {}", m_tempTemplateId);
		return false;
	}

	// state
	const auto& stateOptions = Game::Utils::GetAvailablePedStates();
	auto* stateText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto* stateController = m_menuController->CreateLatestItemController<UiModule::SelectController<Game::PED_STATE>>(
		stateText,
		stateOptions,
		m_tempTemplatePtr->data.state,
		UiModule::SelectControllerOptions{ L"State: #", L"#" }
	);
	stateController->SetSaveCallback([this](Game::PED_STATE newValue) {
		m_tempTemplatePtr->data.state = newValue;
		m_tempTemplatePtr->isSaved = false;
	});
	stateController->SetConverter<PedStateConverter>();

	// state2
	const auto& state2Options = Game::Utils::GetAvailablePedStates2();
	auto* state2Text = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto* state2Controller = m_menuController->CreateLatestItemController<UiModule::SelectController<Game::PED_STATE2>>(
		state2Text,
		state2Options,
		m_tempTemplatePtr->data.state2,
		UiModule::SelectControllerOptions{ L"State 2: #", L"#" }
	);
	state2Controller->SetSaveCallback([this](Game::PED_STATE2 newValue) {
		m_tempTemplatePtr->data.state2 = newValue;
		m_tempTemplatePtr->isSaved = false;
	});
	state2Controller->SetConverter<PedState2Converter>();

	// bit state
	auto* bitStateText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto* bitStateController = m_menuController->CreateLatestItemController<UiModule::EditableController<Game::PED_BIT_STATE>>(
		bitStateText,
		m_tempTemplatePtr->data.bitState,
		UiModule::EditableControllerOptions{ L"Bit state: #0x", L"#" }
	);
	bitStateController->SetSaveCallback([this](Game::PED_BIT_STATE newValue) {
		m_tempTemplatePtr->data.bitState = newValue;
		m_tempTemplatePtr->isSaved = false;
	});
	bitStateController->SetConverter<HexConverter<Game::PED_BIT_STATE>>();

	// bit state 2
	auto* bitState2Text = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto* bitState2Controller = m_menuController->CreateLatestItemController<UiModule::EditableController<Game::PED_BIT_STATE2>>(
		bitState2Text,
		m_tempTemplatePtr->data.bitState2,
		UiModule::EditableControllerOptions{ L"Bit state 2: #0x", L"#" }
	);
	bitState2Controller->SetSaveCallback([this](Game::PED_BIT_STATE2 newValue) {
		m_tempTemplatePtr->data.bitState2 = newValue;
		m_tempTemplatePtr->isSaved = false;
	});
	bitState2Controller->SetConverter<HexConverter<Game::PED_BIT_STATE2>>();

	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::PedTemplateMiscOptionsMenu::Detach()
{
	DestroyMenu();
}

void ModMenuModule::PedTemplateMiscOptionsMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	switch (id) {
	case 0: { // Go back
		ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
		break;
	}
	default:
		break;
	}
}
