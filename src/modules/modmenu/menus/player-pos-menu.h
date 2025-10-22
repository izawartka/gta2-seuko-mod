#pragma once
#include "../common.h"
#include "../menu-base.h"

namespace ModMenuModule {
	class PlayerPosMenu : public MenuBase {
	public:
		PlayerPosMenu();
		virtual ~PlayerPosMenu();

		virtual bool Attach() override;

	private:
		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
	};
}
