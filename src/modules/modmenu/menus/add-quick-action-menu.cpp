#include "add-quick-action-menu.h"
#include "../root.h"
#include "../../../converters/quick-action-type.h"

ModMenuModule::AddQuickActionMenu::AddQuickActionMenu()
{

}

ModMenuModule::AddQuickActionMenu::~AddQuickActionMenu()
{

}

bool ModMenuModule::AddQuickActionMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Add quick action#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	ModMenuModule::ModMenuOptions options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	ModMenuModule::QuickActionManager* quickActionManager = ModMenuModule::QuickActionManager::GetInstance();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);
	
	// key
	auto keyText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_keyController = m_menuController->CreateLatestItemController<UiModule::KeyChangeController>(
		keyText,
		std::nullopt,
		UiModule::KeyChangeControllerOptions{ L"Key: #", L"#" }
	);

	// action type
	auto typeText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto typesList = quickActionManager->GetAllTypes();
	if (typesList.empty()) {
		spdlog::error("No quick action types registered!");
		return false;
	}
	m_actionTypeController = m_menuController->CreateLatestItemController<UiModule::SelectController<QuickActionTypeIndex>>(
		typeText,
		typesList,
		typesList[0],
		UiModule::SelectControllerOptions{ L"Action: #", L"#" }
	);
	m_actionTypeController->SetConverter<QuickActionTypeConverter>();

	// save button
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Save", options.textSize);
	m_saveBtnMenuId = m_menuController->GetLatestMenuItemId();

	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::AddQuickActionMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	if (id == 0) { // Go back
		ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
		return;
	}
	else if (id == m_saveBtnMenuId) { // Save
		OnSave();
		return;
	}
}

void ModMenuModule::AddQuickActionMenu::OnSave()
{
	ModMenuModule::QuickActionManager* quickActionManager = ModMenuModule::QuickActionManager::GetInstance();

	auto keyOpt = m_keyController->GetValue();
	if (!keyOpt.has_value()) {
		spdlog::warn("Cannot add quick action: no key bind set");
		return;
	}
	KeyBindingModule::Key key = keyOpt.value();

	auto actionTypeOpt = m_actionTypeController->GetValue();
	if (!actionTypeOpt.has_value()) {
		spdlog::warn("Cannot add quick action: no action type selected");
		return;
	}
	QuickActionTypeIndex actionType = actionTypeOpt.value();

	quickActionManager->Add(key, actionType);
	ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
}
