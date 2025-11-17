#pragma once
#include "../common.h"
#include "../menu-base.h"
#include "../quick-action-registry.h"

namespace ModMenuModule {
	class AddQuickActionMenu : public MenuBase {
	public:
		AddQuickActionMenu();
		virtual ~AddQuickActionMenu();

		virtual bool Attach() override;

	private:
		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
		void OnSave();

		UiModule::KeyChangeController* m_keyController = nullptr;
		UiModule::SelectController<QuickActionTypeIndex>* m_actionTypeController = nullptr;
		UiModule::MenuItemId m_saveBtnMenuId = 0;
	};
}
