#include "quick-actions-menu.h"
#include "../root.h"
#include "add-quick-action-menu.h"
#include "quick-action-options-menu.h"
#include "../../../converters/key.h"

ModMenuModule::QuickActionsMenu::QuickActionsMenu(size_t page)
{
	m_page = page;
}

ModMenuModule::QuickActionsMenu::~QuickActionsMenu()
{

}

bool ModMenuModule::QuickActionsMenu::Attach()
{
	ModMenuModule::QuickActionManager* quickActionManager = ModMenuModule::QuickActionManager::GetInstance();
	auto actionIds = quickActionManager->GetAll();
	size_t pageCount = std::max((actionIds.size() + ACTIONS_PER_PAGE - 1) / ACTIONS_PER_PAGE, 1U);
	if (m_page >= pageCount && pageCount > 0) {
		m_page = pageCount - 1;
	}

	UiModule::Component* vertCont;
	CreateMenu(GetPageTitle(pageCount), vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Add quick action", options.textSize);

	m_prevPageItemId = -1;
	m_nextPageItemId = -1;
	if (m_page > 0) {
		m_menuController->CreateItem<UiModule::Text>(vertCont, L"Previous page", options.textSize);
		m_prevPageItemId = m_menuController->GetLatestMenuItemId();
	}
	if (m_page < pageCount - 1) {
		m_menuController->CreateItem<UiModule::Text>(vertCont, L"Next page", options.textSize);
		m_nextPageItemId = m_menuController->GetLatestMenuItemId();
	}

	if(actionIds.size() > 0) uiRoot->AddComponent<UiModule::Spacer>(vertCont, 0, options.menuSpacerHeight);

	m_firstActionItemId = -1;
	for (size_t i = m_page * ACTIONS_PER_PAGE; i < std::min(actionIds.size(), (m_page + 1) * ACTIONS_PER_PAGE); i++) {
		ModMenuModule::QuickActionId actionId = actionIds[i];
		std::optional<ModMenuModule::QuickActionInfo> actionInfoOpt = quickActionManager->GetInfo(actionId);
		if (!actionInfoOpt.has_value()) {
			continue;
		}
		ModMenuModule::QuickActionInfo actionInfo = actionInfoOpt.value();
		std::wstring itemLabel = actionInfo.GetLabel() + L" (" + KeyConverter<true>::ConvertToString(actionInfo.key) + L")";
		m_menuController->CreateItem<UiModule::Text>(vertCont, itemLabel, options.textSize);
		if(m_firstActionItemId == -1) {
			m_firstActionItemId = m_menuController->GetLatestMenuItemId();
		}
	}

	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::QuickActionsMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	ModMenuModule::MenuManager* menuManager = ModMenuModule::MenuManager::GetInstance();

	switch (id) {
	case 0: // Go back
		menuManager->RemoveLastMenu();
		return;
	case 1: // Add quick action
		menuManager->AddMenu<AddQuickActionMenu>();
		return;
	default:
		break;
	}

	if (id == m_prevPageItemId) { // Previous page
		MenuId thisMenuId = menuManager->GetTopMenuId();
		menuManager->AddMenu<QuickActionsMenu>(m_page - 1);
		menuManager->RemoveMenu(thisMenuId);
		return;
	}
	else if (id == m_nextPageItemId) { // Next page
		MenuId thisMenuId = menuManager->GetTopMenuId();
		menuManager->AddMenu<QuickActionsMenu>(m_page + 1);
		menuManager->RemoveMenu(thisMenuId);
		return;
	}
	else if (id >= m_firstActionItemId && m_firstActionItemId != -1) {
		ModMenuModule::QuickActionManager* quickActionManager = ModMenuModule::QuickActionManager::GetInstance();
		auto actionIds = quickActionManager->GetAll();
		ModMenuModule::QuickActionId actionId = actionIds[id - m_firstActionItemId + m_page * ACTIONS_PER_PAGE];
		menuManager->AddMenu<QuickActionOptionsMenu>(actionId);
	}
}

std::wstring ModMenuModule::QuickActionsMenu::GetPageTitle(size_t pageCount) const
{
	if (pageCount > 1) {
		return L"#Quick actions (" + std::to_wstring(m_page + 1) + L"/" + std::to_wstring(pageCount) + L")#";
	}
	else {
		return L"#Quick actions";
	}
}
