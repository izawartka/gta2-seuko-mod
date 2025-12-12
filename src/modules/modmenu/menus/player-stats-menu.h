#pragma once
#include "../common.h"
#include "../menu-base.h"

namespace ModMenuModule {
	class PlayerStatsMenu : public MenuBase {
	public:
		PlayerStatsMenu();
		virtual ~PlayerStatsMenu();

	private:
		virtual bool Attach() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
	};
}
