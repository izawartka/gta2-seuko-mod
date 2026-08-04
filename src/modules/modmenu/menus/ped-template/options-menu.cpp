#include "options-menu.h"
#include "../../root.h"
#include "basic-options-menu.h"
#include "logic-options-menu.h"
#include "objective-options-menu.h"
#include "misc-options-menu.h"
#include "saveas-menu.h"
#include "../../toast-manager.h"

ModMenuModule::PedTemplateOptionsMenu::PedTemplateOptionsMenu(TempPedTemplateId tempTemplateId)
{
	m_tempTemplateId = tempTemplateId;
}

ModMenuModule::PedTemplateOptionsMenu::~PedTemplateOptionsMenu()
{

}

bool ModMenuModule::PedTemplateOptionsMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Character template options#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);

	uiRoot->AddComponent<UiModule::Spacer>(vertCont, 0, options.menuSpacerHeight);

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Basic", options.textSize);
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Logic", options.textSize);
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Objective", options.textSize);
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Misc", options.textSize);

	uiRoot->AddComponent<UiModule::Spacer>(vertCont, 0, options.menuSpacerHeight);

	PedTemplatesCheat* pedTemplatesCheat = PedTemplatesCheat::GetInstance();
	m_tempTemplatePtr = pedTemplatesCheat->GetTempTemplate(m_tempTemplateId);
	if (!m_tempTemplatePtr) {
		spdlog::error("PedTemplateOptionsMenu: Failed to get temp template with id {}", m_tempTemplateId);
		return false;
	}

	// save items
	m_saveItemsGroupId = m_menuController->RegisterGroup();
	m_saveItemsIndex = m_menuController->GetNextAddedItemIndex();
	m_saveItemsCont = uiRoot->AddComponent<UiModule::VertCont>(vertCont);
	UpdateSaveItems();

	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::PedTemplateOptionsMenu::Detach()
{
	DestroyMenu();
}

void ModMenuModule::PedTemplateOptionsMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	MenuManager* menuManager = MenuManager::GetInstance();

	switch (id) {
	case 0:
		menuManager->RemoveLastMenu();
		return;
	case 1:
		menuManager->AddMenu<PedTemplateBasicOptionsMenu>(m_tempTemplateId);
		return;
	case 2:
		menuManager->AddMenu<PedTemplateLogicOptionsMenu>(m_tempTemplateId);
		return;
	case 3:
		menuManager->AddMenu<PedTemplateObjectiveOptionsMenu>(m_tempTemplateId);
		return;
	case 4:
		menuManager->AddMenu<PedTemplateMiscOptionsMenu>(m_tempTemplateId);
		return;
	default:
		break;
	}
		
	if (id == m_saveBtnMenuItemId) {
		SaveTemplate();
		return;
	}

	if (id == m_saveAsNewBtnMenuItemId) {
		menuManager->AddMenu<PedTemplateSaveasMenu>(m_tempTemplateId);
		return;
	}

	if (id == m_deleteBtnMenuItemId) {
		DeleteTemplate();
		return;
	}
}

void ModMenuModule::PedTemplateOptionsMenu::SaveTemplate()
{
	PedTemplatesCheat* pedTemplatesCheat = PedTemplatesCheat::GetInstance();

	if (!pedTemplatesCheat->SaveTemplate(m_tempTemplateId, m_tempTemplatePtr->name)) {
		ToastManager::GetInstance()->Show({ L"Failed to save template", ToastType::Error });
		return;
	}

	ToastManager::GetInstance()->Show({ L"Template saved" });
	UpdateSaveItems();
}

void ModMenuModule::PedTemplateOptionsMenu::DeleteTemplate()
{
	PedTemplatesCheat* pedTemplatesCheat = PedTemplatesCheat::GetInstance();

	if (!pedTemplatesCheat->DeleteTemplate(m_tempTemplatePtr->name)) {
		ToastManager::GetInstance()->Show({ L"Failed to delete template", ToastType::Error });
		return;
	}

	m_tempTemplatePtr->isSaved = false;
	ToastManager::GetInstance()->Show({ L"Template deleted" });
	UpdateSaveItems();
}

void ModMenuModule::PedTemplateOptionsMenu::UpdateSaveItems()
{
	UiModule::MenuItemGroupId prevGroupId = m_menuController->GetCurrentGroupId();

	m_menuController->SetCurrentGroupId(m_saveItemsGroupId);
	m_menuController->DeleteGroupItems(m_saveItemsGroupId);
	m_saveBtnMenuItemId = -1;
	m_saveAsNewBtnMenuItemId = -1;
	m_deleteBtnMenuItemId = -1;
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	uiRoot->RemoveChildComponents(m_saveItemsCont);

	m_menuController->SetNextAddedItemIndex(m_saveItemsIndex);

	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	// template name
	std::wstring templateName = m_tempTemplatePtr->name + (m_tempTemplatePtr->isSaved ? L"" : L" (modified)");
	std::wstring fullTemplateNameString = L"Template name: #" + templateName + L"#";
	auto* templateNameMargin = uiRoot->AddComponent<UiModule::Margin>(m_saveItemsCont, options.menuControllerOptions.createdSelectableOptions.markerOffsetX, 0);
	auto* templateNameText = uiRoot->AddComponent<UiModule::Text>(templateNameMargin, fullTemplateNameString, options.textSize);

	// save
	if (!m_tempTemplatePtr->isSaved && !m_tempTemplatePtr->isBuiltin) {
		m_menuController->CreateItem<UiModule::Text>(m_saveItemsCont, L"Save", options.textSize);
		m_saveBtnMenuItemId = m_menuController->GetLatestMenuItemId();
	}

	m_menuController->CreateItem<UiModule::Text>(m_saveItemsCont, L"Save as", options.textSize);
	m_saveAsNewBtnMenuItemId = m_menuController->GetLatestMenuItemId();
	
	// delete
	if (m_tempTemplatePtr->isSaved && !m_tempTemplatePtr->isBuiltin) {
		m_menuController->CreateItem<UiModule::Text>(m_saveItemsCont, L"Delete", options.textSize);
		m_deleteBtnMenuItemId = m_menuController->GetLatestMenuItemId();
	}

	m_menuController->SetCurrentGroupId(prevGroupId);
}
