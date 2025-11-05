#include "menu-controls-menu.h"
#include "../root.h"
#include "../../../converters/key.h"

ModMenuModule::MenuControlsMenu::MenuControlsMenu()
{

}

ModMenuModule::MenuControlsMenu::~MenuControlsMenu()
{

}

bool ModMenuModule::MenuControlsMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Menu controls#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	ModMenuModule::ModMenuOptions options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);

	auto keyToggleMenuText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_menuController->CreateLatestItemController<UiModule::KeyBindChangeController>(
		keyToggleMenuText,
		options.keyBindToggleMenuName,
		options.keyBindToggleMenuDefault,
		UiModule::KeyBindChangeControllerOptions{ L"Toggle menu: #", L"#" }
	);

	UiModule::KeyBindChangeControllerOptions standardUiBindsOptions;
	standardUiBindsOptions.noShift = true;
	standardUiBindsOptions.suffix = L"#";
	
	standardUiBindsOptions.prefix = L"Previous item: #";
	auto keyPrevText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_menuController->CreateLatestItemController<UiModule::KeyBindChangeController>(
		keyPrevText,
		options.menuControllerOptions.keyBindOptions.keyBindPrevName,
		options.menuControllerOptions.keyBindOptions.keyBindPrevDefault,
		standardUiBindsOptions
	);

	standardUiBindsOptions.prefix = L"Next item: #";
	auto keyNextText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_menuController->CreateLatestItemController<UiModule::KeyBindChangeController>(
		keyNextText,
		options.menuControllerOptions.keyBindOptions.keyBindNextName,
		options.menuControllerOptions.keyBindOptions.keyBindNextDefault,
		standardUiBindsOptions
	);

	standardUiBindsOptions.prefix = L"Action (select): #";
	auto keyActionText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_menuController->CreateLatestItemController<UiModule::KeyBindChangeController>(
		keyActionText,
		options.menuControllerOptions.keyBindOptions.keyBindActionName,
		options.menuControllerOptions.keyBindOptions.keyBindActionDefault,
		standardUiBindsOptions
	);

	auto resetInfoMargin = uiRoot->AddComponent<UiModule::Margin>(
		vertCont, 
		options.menuControllerOptions.createdSelectableOptions.markerOffsetX, 
		RESET_TOOLTIP_OFFSET_Y
	);

	std::wstring resetInfo = 
		L"Hold " +
		KeyConverter<true>::ConvertToString(options.keyBindToggleMenuDefault) +
		L" to reset all.";

	uiRoot->AddComponent<UiModule::Text>(
		resetInfoMargin,
		resetInfo,
		options.textSize
	);

	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::MenuControlsMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	switch (id) {
	case 0: // Go back
		ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
		break;
	default:
		break;
	}
}
