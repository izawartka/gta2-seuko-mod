#pragma once
#include "../common.h"
#include "../menu-base.h"

namespace ModMenuModule {
	class PlayerStatsMenu : public MenuBase {
	public:
		PlayerStatsMenu();
		virtual ~PlayerStatsMenu();

		virtual bool Attach() override;

	private:
		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
	};
}
