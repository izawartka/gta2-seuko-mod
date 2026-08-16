#pragma once
#include "../common.h"
#include "../menu-base.h"

namespace ModMenuModule {
	class SeukomodOptionsMenu : public MenuBase {
	public:
		SeukomodOptionsMenu();
		virtual ~SeukomodOptionsMenu();

	private:
		virtual bool Attach() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
	};
}
