#pragma once
#include "../common.h"
#include "../menu-base.h"

namespace ModMenuModule {
	class WorldMenu : public MenuBase {
	public:
		WorldMenu();
		virtual ~WorldMenu();

	private:
		virtual bool Attach() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
	};
}
