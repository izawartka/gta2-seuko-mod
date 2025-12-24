#pragma once
#include "../common.h"
#include "../menu-base.h"

static constexpr size_t SAVED_CARS_PER_PAGE = 8;

namespace ModMenuModule {
	class SavedVehiclesMenu : public MenuBase {
	public:
		SavedVehiclesMenu(size_t page = 0);
		virtual ~SavedVehiclesMenu();

	private:
		virtual bool Attach() override;
		virtual void Detach() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
		std::wstring GetPageTitle(size_t pageCount) const;

		UiModule::MenuItemId m_firstCarItemId = -1;
		size_t m_page;
		UiModule::MenuItemId m_prevPageItemId = -1;
		UiModule::MenuItemId m_nextPageItemId = -1;
		std::vector<std::wstring> m_shownCarNames;
	};
}
