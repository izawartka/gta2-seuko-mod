#pragma once
#include "../common.h"
#include "../menu-base.h"

namespace ModMenuModule {
	class CameraMenu : public MenuBase {
	public:
		CameraMenu();
		virtual ~CameraMenu();

		virtual bool Attach() override;

	private:
		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
	};
}
