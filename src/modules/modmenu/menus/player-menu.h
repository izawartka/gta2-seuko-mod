#pragma once
#include "../common.h"
#include "../menu-base.h"

namespace ModMenuModule {
	class PlayerMenu : public MenuBase {
	public:
		PlayerMenu();
		virtual ~PlayerMenu();

		virtual bool Attach() override;
		virtual void Detach() override;

		virtual void OnShow() override;
		virtual void OnHide() override;

	private:
		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;

		UiModule::VarTextSelectController<short>* m_wantedLevelController = nullptr;
	};
}
