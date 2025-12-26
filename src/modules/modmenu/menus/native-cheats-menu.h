#pragma once
#include "../common.h"
#include "../menu-base.h"
#include "../cheats/native-cheats-keeper.h"

namespace ModMenuModule {
	class NativeCheatsMenu : public MenuBase {
	public:
		NativeCheatsMenu();
		virtual ~NativeCheatsMenu();

	private:
		virtual bool Attach() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;

		const std::vector<NativeCheatCategoryDef>& m_categories = NativeCheatsKeeperCheat::GetAllNativeCheatCategoryDefs();
		UiModule::MenuItemId m_firstCategoryItemId = -1;
	};
}
