#pragma once
#include "../common.h"
#include "../cheat-support.h"
#include "../menu-base.h"
#include "../cheats/native-cheats-keeper.h"

static constexpr size_t CHEATS_PER_PAGE = 9;
static constexpr Game::SCR_f NATIVE_CHEATS_OFFSET_Y = Game::Utils::FromFloat(12.0f);

namespace ModMenuModule {
	class NativeCheatsCategoryMenu : public MenuBase, public CheatSupport {
	public:
		NativeCheatsCategoryMenu(const NativeCheatCategoryDef& categoryDef, size_t page = 0);
		virtual ~NativeCheatsCategoryMenu();

		virtual bool Attach() override;
		virtual void Detach() override;

		virtual void OnShow() override;
		virtual void OnHide() override;

	private:
		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
		std::wstring GetTitle() const;

		const NativeCheatCategoryDef& m_categoryDef;
		const std::vector<NativeCheatDef>& m_cheats;
		size_t m_page;
		size_t m_pageCount;
		std::vector<UiModule::VarTextController<bool, bool>*> m_cheatValueControllers = {};
		UiModule::MenuItemId m_prevPageItemId = -1;
		UiModule::MenuItemId m_nextPageItemId = -1;
	};
}
