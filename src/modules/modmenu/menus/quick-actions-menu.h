#pragma once
#include "../common.h"
#include "../menu-base.h"

namespace ModMenuModule {
	class QuickActionsMenu : public MenuBase {
	public:
		QuickActionsMenu();
		virtual ~QuickActionsMenu();

		virtual bool Attach() override;

	private:
		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;

		UiModule::MenuItemId m_firstActionItemId = -1;
	};
}
