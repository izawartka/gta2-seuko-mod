#include "quick-action-options-menu.h"
#include "../root.h"
#include "../../../converters/quick-action-type.h"

ModMenuModule::QuickActionOptionsMenu::QuickActionOptionsMenu(QuickActionId actionId)
{
	m_actionId = actionId;
	m_keySegment = CreateSegment<QuickActionKeySegment>(actionId);
}

ModMenuModule::QuickActionOptionsMenu::~QuickActionOptionsMenu()
{
	DeleteSegment(m_keySegment);
}

bool ModMenuModule::QuickActionOptionsMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Quick action options#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();
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
	AttachSegment(m_keySegment, this, vertCont);
	m_keySegment->SetSegmentData({ m_actionInfo->key });

	// save button
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Save", options.textSize);
	m_saveBtnMenuId = m_menuController->GetLatestMenuItemId();

	// delete button
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Delete", options.textSize);
	m_deleteBtnMenuId = m_menuController->GetLatestMenuItemId();

	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::QuickActionOptionsMenu::Detach()
{
	DetachSegment(m_keySegment);
	DestroyMenu();
}

void ModMenuModule::QuickActionOptionsMenu::OnShow()
{
	SetSegmentsVisible(true);
}

void ModMenuModule::QuickActionOptionsMenu::OnHide()
{
	SetSegmentsVisible(false);
}

void ModMenuModule::QuickActionOptionsMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	ModMenuModule::QuickActionManager* quickActionManager = ModMenuModule::QuickActionManager::GetInstance();
	ModMenuModule::MenuManager* menuManager = ModMenuModule::MenuManager::GetInstance();

	if (id == 0) { // Go back
		menuManager->RemoveLastMenu();
	} 
	else if (id == m_saveBtnMenuId) { // Save
		if (!SaveAction()) return;
		menuManager->RemoveLastMenu();
	}
	else if (id == m_deleteBtnMenuId) { // Delete
		quickActionManager->Remove(m_actionId);
		menuManager->RemoveLastMenu();
	}
}

bool ModMenuModule::QuickActionOptionsMenu::SaveAction()
{
	if (!m_keySegment) {
		spdlog::warn("Cannot save quick action: key segment not initialized");
		return false;
	}
	auto keySegmentData = m_keySegment->GetSegmentData();
	if (!keySegmentData.has_value()) {
		spdlog::warn("Cannot save quick action: no key selected");
		ToastManager::GetInstance()->Show({ L"Cannot save quick action: no key bind set", ToastType::Warning });
		return false;
	}
	m_actionInfo->key = keySegmentData.value().key;

	QuickActionManager* quickActionManager = QuickActionManager::GetInstance();
	quickActionManager->Update(m_actionId, m_actionInfo.value());
	return true;
}
