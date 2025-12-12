#pragma once
#include "../common.h"
#include "../menu-base.h"

static constexpr size_t ACTIONS_PER_PAGE = 8;

namespace ModMenuModule {
	class QuickActionsMenu : public MenuBase {
	public:
		QuickActionsMenu(size_t page = 0);
		virtual ~QuickActionsMenu();

	private:
		virtual bool Attach() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
		std::wstring GetPageTitle(size_t pageCount) const;

		UiModule::MenuItemId m_firstActionItemId = -1;
		size_t m_page;
		UiModule::MenuItemId m_prevPageItemId = -1;
		UiModule::MenuItemId m_nextPageItemId = -1;
	};
}
