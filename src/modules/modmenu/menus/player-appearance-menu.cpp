#include "player-appearance-menu.h"
#include "../cheats/player-appearance.h"
#include "../../../converters/ped-remap.h"
#include "../../../converters/ped-graphic-type.h"
#include "../../../converters/yes-no.h"
#include "../root.h"

ModMenuModule::PlayerAppearanceMenu::PlayerAppearanceMenu()
{

}

ModMenuModule::PlayerAppearanceMenu::~PlayerAppearanceMenu()
{

}

bool ModMenuModule::PlayerAppearanceMenu::Attach()
{

	UiModule::Component* vertCont;
	CreateMenu(L"#Player appearance#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);

	PlayerAppearanceCheat* playerAppearanceCheat = PlayerAppearanceCheat::GetInstance();
	if (!playerAppearanceCheat->IsEnabled()) {
		playerAppearanceCheat->SetEnabled(true);
	}

	// ped remap
	UiModule::Text* remapText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_remapController = m_menuController->CreateLatestItemController<UiModule::SelectController<Game::PED_REMAP>>(
		remapText,
		Game::Utils::GetAvailablePedRemaps(),
		std::nullopt,
		UiModule::SelectControllerOptions{ L"Ped Remap: #", L"#" }
	);
	m_remapController->SetConverter<PedRemapConverter>();
	m_remapController->SetSaveCallback([playerAppearanceCheat](Game::PED_REMAP newRemap) {
		playerAppearanceCheat->SetRemap(newRemap);
	});

	// ped graphic type
	UiModule::Text* graphicTypeText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_graphicTypeController = m_menuController->CreateLatestItemController<UiModule::SelectController<Game::PED_GRAPHIC_TYPE>>(
		graphicTypeText,
		Game::Utils::GetAvailablePedGraphicTypes(),
		std::nullopt,
		UiModule::SelectControllerOptions{ L"Ped Graphic Type: #", L"#" }
	);
	m_graphicTypeController->SetConverter<PedGraphicTypeConverter>();
	m_graphicTypeController->SetSaveCallback([playerAppearanceCheat](Game::PED_GRAPHIC_TYPE newGraphicType) {
		playerAppearanceCheat->SetGraphicType(newGraphicType);
	});

	// lock
	UiModule::Text* lockText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto* lockController = m_menuController->CreateLatestItemController<UiModule::VarTextSelectController<bool, bool>>(
		lockText,
		[playerAppearanceCheat]() {
			return playerAppearanceCheat->IsRemapLocked() && playerAppearanceCheat->IsGraphicTypeLocked();
		},
		UiModule::VarTextSelectOptionList<bool>{ false, true },
		UiModule::VarTextSelectControllerOptions{ L"Lock Appearance: #", L"#" }
	);
	lockController->SetConverter<YesNoConverter>();
	lockController->SetCustomSaveCallback([playerAppearanceCheat](bool lockAppearance) {
		playerAppearanceCheat->SetRemapLocked(lockAppearance);
		playerAppearanceCheat->SetGraphicTypeLocked(lockAppearance);
	});

	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::PlayerAppearanceMenu::OnShow()
{
	AddEventListener<ModMenuModule::PlayerAppearanceUpdateEvent>(&PlayerAppearanceMenu::OnPlayerAppearanceUpdate);
	ForceUpdateControllers();
}

void ModMenuModule::PlayerAppearanceMenu::OnHide()
{
	RemoveEventListener<ModMenuModule::PlayerAppearanceUpdateEvent>();
}

void ModMenuModule::PlayerAppearanceMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	switch (id) {
	case 0: // Go back
		ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
		break;
	default:
		break;
	}
}

void ModMenuModule::PlayerAppearanceMenu::OnPlayerAppearanceUpdate(ModMenuModule::PlayerAppearanceUpdateEvent& event)
{
	spdlog::debug("PlayerAppearanceMenu::OnPlayerAppearanceUpdate: Received PlayerAppearanceUpdateEvent, remap: {}, graphicType: {}",
		event.GetRemap().has_value() ? std::to_string(static_cast<int>(event.GetRemap().value())) : "null",
		event.GetGraphicType().has_value() ? std::to_string(static_cast<int>(event.GetGraphicType().value())) : "null"
	);

	if (m_remapController) {
		m_remapController->SetValue(event.GetRemap());
	}

	if (m_graphicTypeController) {
		m_graphicTypeController->SetValue(event.GetGraphicType());
	}
}

void ModMenuModule::PlayerAppearanceMenu::ForceUpdateControllers()
{
	PlayerAppearanceCheat* playerAppearanceCheat = PlayerAppearanceCheat::GetInstance();

	if (m_remapController) {
		m_remapController->SetValue(playerAppearanceCheat->GetRemap());
	}

	if (m_graphicTypeController) {
		m_graphicTypeController->SetValue(playerAppearanceCheat->GetGraphicType());
	}
}
