#pragma once
#include "../common.h"
#include "../menu-base.h"

namespace ModMenuModule {
	class NativeCheatsMenu : public MenuBase {
	public:
		NativeCheatsMenu();
		virtual ~NativeCheatsMenu();

	private:
		virtual bool Attach() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;

		UiModule::MenuItemId m_firstCategoryItemId = -1;
	};
}
