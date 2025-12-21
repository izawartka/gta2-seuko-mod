#pragma once
#include "../common.h"
#include "../menu-base.h"

namespace ModMenuModule {
	class VersionMenu : public MenuBase {
	public:
		VersionMenu();
		virtual ~VersionMenu();

	private:
		virtual bool Attach() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
	};
}
