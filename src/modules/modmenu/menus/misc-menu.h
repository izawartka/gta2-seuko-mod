#pragma once
#include "../common.h"
#include "../menu-base.h"

namespace ModMenuModule {
	class MiscMenu : public MenuBase {
	public:
		MiscMenu();
		virtual ~MiscMenu();

	private:
		virtual bool Attach() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
		void QuickSave();
	};
}
