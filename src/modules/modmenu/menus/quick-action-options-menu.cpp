#include "quick-action-options-menu.h"
#include "../root.h"
#include "../../../converters/quick-action-type.h"

ModMenuModule::QuickActionOptionsMenu::QuickActionOptionsMenu(QuickActionId actionId)
{
	m_actionId = actionId;
}

ModMenuModule::QuickActionOptionsMenu::~QuickActionOptionsMenu()
{

}

bool ModMenuModule::QuickActionOptionsMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Quick action options#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	ModMenuModule::ModMenuOptions options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	ModMenuModule::QuickActionManager* quickActionManager = ModMenuModule::QuickActionManager::GetInstance();
	m_actionInfo = quickActionManager->GetInfo(m_actionId);
	if(!m_actionInfo.has_value()) {
		return false;
	}

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);

	// type
	auto typeName = QuickActionTypeConverter::ConvertToString(m_actionInfo->typeIndex);
	auto typeString = L"Type: #" + typeName + L"#";
	auto typeMargin = uiRoot->AddComponent<UiModule::Margin>(vertCont, options.menuControllerOptions.createdSelectableOptions.markerOffsetX, 0);
	uiRoot->AddComponent<UiModule::Text>(typeMargin, typeString, options.textSize);

	// name
	auto nameText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto nameController = m_menuController->CreateLatestItemController<UiModule::EditableController<std::wstring>>(
		nameText,
		m_actionInfo->GetLabel(),
		UiModule::EditableControllerOptions{ L"Name: #", L"#" }
	);
	nameController->SetSaveCallback([this, quickActionManager](std::wstring newName) {
		m_actionInfo->customLabel = newName;
	});

	// key
	auto keyText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto keyController = m_menuController->CreateLatestItemController<UiModule::KeyChangeController>(
		keyText,
		m_actionInfo->key,
		UiModule::KeyChangeControllerOptions{ L"Key: #", L"#" }
	);
	keyController->SetSaveCallback([this, quickActionManager](KeyBindingModule::Key newKey) {
		m_actionInfo->key = newKey;
	});

	// save button
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Save", options.textSize);
	m_saveBtnMenuId = m_menuController->GetLatestMenuItemId();

	// delete button
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Delete", options.textSize);
	m_deleteBtnMenuId = m_menuController->GetLatestMenuItemId();

	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::QuickActionOptionsMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	ModMenuModule::QuickActionManager* quickActionManager = ModMenuModule::QuickActionManager::GetInstance();
	ModMenuModule::MenuManager* menuManager = ModMenuModule::MenuManager::GetInstance();

	if (id == 0) { // Go back
		menuManager->RemoveLastMenu();
	} 
	else if (id == m_saveBtnMenuId) { // Save
		quickActionManager->Update(m_actionId, m_actionInfo.value());
		menuManager->RemoveLastMenu();
	}
	else if (id == m_deleteBtnMenuId) { // Delete
		quickActionManager->Remove(m_actionId);
		menuManager->RemoveLastMenu();
	}
}
