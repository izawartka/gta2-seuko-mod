#include "camera-menu.h"
#include "../../../converters/camera-easy-mode.h"
#include "../utils/camera-easy-mode-utils.h"
#include "camera-pos-menu.h"
#include "camera-rendering-menu.h"
#include "camera-advanced-menu.h"
#include "../cheats/camera/freecam.h"
#include "../../../converters/enabled-disabled.h"
#include "../root.h"

ModMenuModule::CameraMenu::CameraMenu()
{

}

ModMenuModule::CameraMenu::~CameraMenu()
{

}

bool ModMenuModule::CameraMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Camera#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Camera position", options.textSize);
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Advanced rendering options", options.textSize);
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Advanced camera options", options.textSize);

	// easy mode
	UiModule::Text* modeText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto modeController = m_menuController->CreateLatestItemController<
		UiModule::VarTextSelectController<
			Utils::CameraEasyMode::CameraEasyMode, 
			Utils::CameraEasyMode::CameraEasyMode
		>
	>(
		modeText,
		Utils::CameraEasyMode::GetCurrentMode,
		Utils::CameraEasyMode::GetSelectableModes(),
		UiModule::VarTextSelectControllerOptions{ L"Camera Mode: #", L"#" }
	);
	modeController->SetConverter<CameraEasyModeConverter>();
	modeController->SetCustomSaveCallback(Utils::CameraEasyMode::SetCurrentMode);

	// freecam
	FreecamCheat* freecamCheat = FreecamCheat::GetInstance();
	UiModule::Text* freecamText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_freecamCheatController = m_menuController->CreateLatestItemController<UiModule::SelectController<bool>>(
		freecamText,
		UiModule::SelectOptionList<bool>{ false, true },
		std::nullopt,
		UiModule::SelectControllerOptions{ L"Freecam: #", L"#" }
	);
	m_freecamCheatController->SetConverter<EnabledDisabledConverter>();
	m_freecamCheatController->SetSaveCallback([freecamCheat](bool newValue) {
		if (newValue) freecamCheat->SetEnabled(true);
		else freecamCheat->ResetAndDisable();
	});
	
	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::CameraMenu::OnShow()
{
	AddEventListener<ModMenuModule::CheatStateEvent>(&CameraMenu::OnCheatStateChange);
	UpdateCheatStates();
}

void ModMenuModule::CameraMenu::OnHide()
{
	RemoveEventListener<ModMenuModule::CheatStateEvent>();
}

void ModMenuModule::CameraMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	switch (id) {
	case 0: // Go back
		ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
		break;
	case 1: // Camera position
		ModMenuModule::MenuManager::GetInstance()->AddMenu<CameraPosMenu>();
		break;
	case 2: // Advanced rendering options
		ModMenuModule::MenuManager::GetInstance()->AddMenu<CameraRenderingMenu>();
		break;
	case 3: // Advanced camera settings
		ModMenuModule::MenuManager::GetInstance()->AddMenu<CameraAdvancedMenu>();
		break;
	default:
		break;
	}
}

void ModMenuModule::CameraMenu::OnCheatStateChange(CheatStateEvent& event)
{
	if (event.GetCheatType() == typeid(FreecamCheat)) {
		m_freecamCheatController->SetValue(event.IsEnabled());
	}
}

void ModMenuModule::CameraMenu::UpdateCheatStates()
{
	m_freecamCheatController->SetValue(FreecamCheat::GetInstance()->IsEnabled());
}
