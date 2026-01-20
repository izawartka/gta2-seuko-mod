#include "camera-menu.h"
#include "../../../converters/camera-easy-mode.h"
#include "../utils/camera-easy-mode-utils.h"
#include "camera-pos-menu.h"
#include "camera-rendering-menu.h"
#include "camera-advanced-menu.h"
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
	
	SetPreviousSelectedIndex();

	return true;
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
