#pragma once
#include "../common.h"
#include "../menu-base.h"
#include "../quick-action-manager.h"

namespace ModMenuModule {
	class QuickActionOptionsMenu : public MenuBase {
	public:
		QuickActionOptionsMenu(QuickActionId actionId);
		virtual ~QuickActionOptionsMenu();

		virtual bool Attach() override;

	private:
		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;

		QuickActionId m_actionId;
		std::optional<QuickActionInfo> m_actionInfo = std::nullopt;
		UiModule::MenuItemId m_saveBtnMenuId = 0;
		UiModule::MenuItemId m_deleteBtnMenuId = 0;
	};
}
