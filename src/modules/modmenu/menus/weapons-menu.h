#pragma once
#include "../common.h"
#include "../menu-base.h"

namespace ModMenuModule {
	class WeaponsMenu : public MenuBase {
	public:
		WeaponsMenu();
		virtual ~WeaponsMenu();

		virtual bool Attach() override;
		virtual void Detach() override;

		virtual void OnShow() override;
		virtual void OnHide() override;

	private:
		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;

		UiModule::VarTextEditableController<short>* m_ammoController = nullptr;
	};
}
