#pragma once
#include "../common.h"
#include "../cheat-support.h"
#include "../menu-base.h"
#include "../cheats/native-cheats-keeper.h"

static constexpr Game::SCR_f NATIVE_CHEATS_CATEGORIES_OFFSET_Y = Game::Utils::FromFloat(12.0f);

namespace ModMenuModule {
	class NativeCheatsMenu : public MenuBase, public CheatSupport {
	public:
		NativeCheatsMenu();
		virtual ~NativeCheatsMenu();

		virtual bool Attach() override;

	private:
		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;

		const std::vector<NativeCheatCategoryDef>& m_categories = NativeCheatsKeeperCheat::GetAllNativeCheatCategories();
		UiModule::MenuItemId m_firstCategoryItemId = -1;
	};
}
