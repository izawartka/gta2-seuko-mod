#pragma once
#include "../common.h"
#include "../menu-base.h"
#include "../cheats/native-cheats-keeper.h"

static constexpr size_t CHEATS_PER_PAGE = 9;

namespace ModMenuModule {
	class NativeCheatsCategoryMenu : public MenuBase {
	public:
		NativeCheatsCategoryMenu(const NativeCheatCategoryDef& categoryDef, size_t page = 0);
		virtual ~NativeCheatsCategoryMenu();

	private:
		virtual bool Attach() override;
		virtual void Detach() override;

		virtual void OnShow() override;
		virtual void OnHide() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
		std::wstring GetPageTitle() const;

		const NativeCheatCategoryDef& m_categoryDef;
		const std::vector<const NativeCheatDef*>& m_cheats;
		size_t m_page;
		size_t m_pageCount;
		std::vector<UiModule::VarTextController<bool, bool>*> m_cheatValueControllers = {};
		UiModule::MenuItemId m_prevPageItemId = -1;
		UiModule::MenuItemId m_nextPageItemId = -1;
	};
}
