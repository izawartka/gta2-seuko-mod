#pragma once
#include "../common.h"
#include "../menu-base.h"

namespace ModMenuModule {
	class MainMenu : public MenuBase {
	public:
		MainMenu();
		virtual ~MainMenu();

	private:
		virtual bool Attach() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
	};
}
