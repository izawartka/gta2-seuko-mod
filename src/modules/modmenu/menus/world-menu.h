#pragma once
#include "../common.h"
#include "../menu-base.h"

namespace ModMenuModule {
	class WorldMenu : public MenuBase {
	public:
		WorldMenu();
		virtual ~WorldMenu();

		virtual bool Attach() override;

	private:
		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
	};
}
