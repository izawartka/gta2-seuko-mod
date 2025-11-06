#pragma once
#include "../common.h"
#include "../menu-base.h"

namespace ModMenuModule {
	class MiscMenu : public MenuBase {
	public:
		MiscMenu();
		virtual ~MiscMenu();

		virtual bool Attach() override;

	private:
		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
		void QuickSave();
	};
}
