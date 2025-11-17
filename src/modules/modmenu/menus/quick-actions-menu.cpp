#include "quick-actions-menu.h"
#include "../root.h"
#include "add-quick-action-menu.h"
#include "quick-action-options-menu.h"
#include "../../../converters/key.h"

ModMenuModule::QuickActionsMenu::QuickActionsMenu()
{

}

ModMenuModule::QuickActionsMenu::~QuickActionsMenu()
{

}

bool ModMenuModule::QuickActionsMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Quick actions#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	ModMenuModule::ModMenuOptions options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Add quick action", options.textSize);

	ModMenuModule::QuickActionManager* quickActionManager = ModMenuModule::QuickActionManager::GetInstance();
	auto actionIds = quickActionManager->GetAll();

	for (ModMenuModule::QuickActionId actionId : actionIds) {
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
	switch (id) {
	case 0: // Go back
		ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
		return;
	case 1: // Add quick action
		ModMenuModule::MenuManager::GetInstance()->AddMenu<AddQuickActionMenu>();
		return;
	default:
		break;
	}

	if (id >= m_firstActionItemId && m_firstActionItemId != -1) {
		ModMenuModule::QuickActionManager* quickActionManager = ModMenuModule::QuickActionManager::GetInstance();
		auto actionIds = quickActionManager->GetAll();
		ModMenuModule::QuickActionId actionId = actionIds[id - m_firstActionItemId];
		ModMenuModule::MenuManager::GetInstance()->AddMenu<QuickActionOptionsMenu>(actionId);
	}
}
