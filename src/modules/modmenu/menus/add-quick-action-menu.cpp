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
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	ModMenuModule::QuickActionManager* quickActionManager = ModMenuModule::QuickActionManager::GetInstance();

	m_menuController->SetCurrentGroupId(0); // main group
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
	m_actionTypeController->SetSaveCallback(std::bind(&AddQuickActionMenu::OnActionTypeChange, this, std::placeholders::_1));

	// segment container
	m_segmentBaseIndex = m_menuController->GetNextAddedItemIndex();
	m_segmentContainer = uiRoot->AddComponent<UiModule::VertCont>(vertCont);

	// save button
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Save", options.textSize);
	m_saveBtnMenuId = m_menuController->GetLatestMenuItemId();

	CreateSegment(typesList[0]);
	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::AddQuickActionMenu::Detach()
{
	DestroySegment();
	DestroyMenu();
}

void ModMenuModule::AddQuickActionMenu::OnShow()
{
	if (m_segmentInstance != nullptr) {
		m_segmentInstance->OnShow();
	}
}

void ModMenuModule::AddQuickActionMenu::OnHide()
{
	if (m_segmentInstance != nullptr) {
		m_segmentInstance->OnHide();
	}
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

void ModMenuModule::AddQuickActionMenu::OnActionTypeChange(QuickActionTypeIndex actionType)
{
	DestroySegment();
	CreateSegment(actionType);
}

void ModMenuModule::AddQuickActionMenu::CreateSegment(QuickActionTypeIndex actionType)
{
	if (m_segmentInstance != nullptr) return;

	if (!ModMenuModule::QuickActionManager::HasSegmentFactory(actionType)) {
		return;
	}

	m_segmentInstance = ModMenuModule::QuickActionManager::CreateSegment(actionType);
	m_menuController->SetNextAddedItemIndex(m_segmentBaseIndex);
	m_menuController->SetCurrentGroupId(1); // segment group

	m_segmentInstance->Attach(this, m_segmentContainer);
	if(m_visible) {
		m_segmentInstance->OnShow();
	}
}

void ModMenuModule::AddQuickActionMenu::DestroySegment()
{
	if (m_segmentInstance == nullptr) return;

	if (m_visible) {
		m_segmentInstance->OnHide();
	}
	m_segmentInstance->Detach();
	m_menuController->DeleteGroupItems(1); // segment group
	delete m_segmentInstance;
	m_segmentInstance = nullptr;
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

	QuickActionId actionId = quickActionManager->Add(key, actionType);
	if(m_segmentInstance) quickActionManager->SetDataFromSegmentData(actionId, m_segmentInstance);
	ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
}
