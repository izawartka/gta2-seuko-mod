#include "add-quick-action-menu.h"
#include "../root.h"
#include "../../../converters/quick-action-type.h"

ModMenuModule::AddQuickActionMenu::AddQuickActionMenu()
{

}

ModMenuModule::AddQuickActionMenu::~AddQuickActionMenu()
{
	DestroySegment();
}

bool ModMenuModule::AddQuickActionMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Add quick action#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	QuickActionManager* quickActionManager = QuickActionManager::GetInstance();
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);
	
	// key
	auto keyText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_keyController = m_menuController->CreateLatestItemController<UiModule::KeyChangeController>(
		keyText,
		m_lastSelectedKey,
		UiModule::KeyChangeControllerOptions{ L"Key: #", L"#" }
	);

	// action type
	auto typeText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto& typesList = QuickActionManager::GetAllTypes(true);
	if (typesList.empty()) {
		spdlog::error("No quick action types registered!");
		return false;
	}

	auto selectedType = typesList[0];
	if (m_lastSelectedActionType.has_value() && std::find(typesList.begin(), typesList.end(), m_lastSelectedActionType.value()) != typesList.end()) {
		selectedType = m_lastSelectedActionType.value();
	}

	m_actionTypeController = m_menuController->CreateLatestItemController<UiModule::SelectController<QuickActionTypeIndex>>(
		typeText,
		typesList,
		selectedType,
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

	CreateSegment(selectedType);
	AttachSegment();

	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::AddQuickActionMenu::Detach()
{
	if (m_keyController) {
		m_lastSelectedKey = m_keyController->GetValue();
	}
	if (m_actionTypeController) {
		m_lastSelectedActionType = m_actionTypeController->GetValue();
	}

	SaveCurrentSelectedIndex();
	DetachAllSegments();
	DestroyMenu();
}

void ModMenuModule::AddQuickActionMenu::OnShow()
{
	SetSegmentsVisible(true);
}

void ModMenuModule::AddQuickActionMenu::OnHide()
{
	SetSegmentsVisible(false);
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
	else if (m_segmentInstance) {
		m_segmentInstance->OnPassedMenuAction(item, id);
	}
}

void ModMenuModule::AddQuickActionMenu::OnActionTypeChange(QuickActionTypeIndex actionType)
{
	DestroySegment();
	CreateSegment(actionType);
	AttachSegment();
}

void ModMenuModule::AddQuickActionMenu::CreateSegment(QuickActionTypeIndex actionType)
{
	if (m_segmentInstance != nullptr) return;

	if (!ModMenuModule::QuickActionManager::HasSegmentFactory(actionType)) {
		return;
	}

	m_segmentInstance = ModMenuModule::QuickActionManager::CreateSegment(actionType);
	SegmentSupport::AddSegment(m_segmentInstance);
}

void ModMenuModule::AddQuickActionMenu::AttachSegment()
{
	if (m_segmentInstance == nullptr) return;

	m_menuController->SetNextAddedItemIndex(m_segmentBaseIndex);
	SegmentSupport::AttachSegment(
		m_segmentInstance,
		this,
		m_segmentContainer
	);
}

void ModMenuModule::AddQuickActionMenu::DetachSegment()
{
	if (m_segmentInstance == nullptr) return;

	SegmentSupport::DetachSegment(m_segmentInstance);
}

void ModMenuModule::AddQuickActionMenu::DestroySegment()
{
	if (m_segmentInstance == nullptr) return;

	SegmentSupport::DeleteSegment(m_segmentInstance);
}

void ModMenuModule::AddQuickActionMenu::OnSave()
{
	ModMenuModule::QuickActionManager* quickActionManager = ModMenuModule::QuickActionManager::GetInstance();

	auto& keyOpt = m_keyController->GetValue();
	if (!keyOpt.has_value()) {
		ToastManager* toastManager = ToastManager::GetInstance();
		toastManager->Show({ L"Cannot add quick action: no key bind set", ToastType::Warning });
		spdlog::warn("Cannot add quick action: no key bind set");
		return;
	}
	KeyBindingModule::Key key = keyOpt.value();

	auto& actionTypeOpt = m_actionTypeController->GetValue();
	if (!actionTypeOpt.has_value()) {
		spdlog::warn("Cannot add quick action: no action type selected");
		return;
	}
	QuickActionTypeIndex actionType = actionTypeOpt.value();

	if (m_segmentInstance && !m_segmentInstance->ValidateSegment()) {
		spdlog::warn("Cannot add quick action: segment validation failed");
		return;
	}

	QuickActionId actionId = quickActionManager->Add(key, actionType);
	if(m_segmentInstance) quickActionManager->SetDataFromSegmentData(actionId, m_segmentInstance);
	ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
}
