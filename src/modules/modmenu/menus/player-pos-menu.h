#pragma once
#include "../common.h"
#include "../menu-base.h"

namespace ModMenuModule {
	class PlayerPosMenu : public MenuBase {
	public:
		PlayerPosMenu();
		virtual ~PlayerPosMenu();

	private:
		virtual bool Attach() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
	};
}
