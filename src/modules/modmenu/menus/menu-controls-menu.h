#pragma once
#include "../common.h"
#include "../menu-base.h"

namespace ModMenuModule {
	class MenuControlsMenu : public MenuBase {
	public:
		MenuControlsMenu();
		virtual ~MenuControlsMenu();

	private:
		virtual bool Attach() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
	};
}
