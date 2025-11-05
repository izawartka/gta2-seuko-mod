#pragma once
#include "../common.h"
#include "../menu-base.h"

static constexpr Game::SCR_f RESET_TOOLTIP_OFFSET_Y = Game::Utils::FromFloat(12.0f);

namespace ModMenuModule {
	class MenuControlsMenu : public MenuBase {
	public:
		MenuControlsMenu();
		virtual ~MenuControlsMenu();

		virtual bool Attach() override;

	private:
		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
	};
}
