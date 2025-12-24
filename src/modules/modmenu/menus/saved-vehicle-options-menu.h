#pragma once
#include "../common.h"
#include "../menu-base.h"

namespace ModMenuModule {
	class SavedVehicleOptionsMenu : public MenuBase {
	public:
		SavedVehicleOptionsMenu(const std::wstring& savedCarName);
		virtual ~SavedVehicleOptionsMenu();

	private:
		virtual bool Attach() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;

		std::wstring m_savedCarName;
	};
}
