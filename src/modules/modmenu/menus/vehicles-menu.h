#pragma once
#include "../common.h"
#include "../menu-base.h"

namespace ModMenuModule {
	class VehiclesMenu : public MenuBase {
	public:
		VehiclesMenu();
		virtual ~VehiclesMenu();

		virtual bool Attach() override;

	private:
		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
	};
}
