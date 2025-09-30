#pragma once
#include "../common.h"
#include "../menu-base.h"

namespace ModMenuModule {
	class PlayerPosMenu : public MenuBase {
	public:
		PlayerPosMenu();
		virtual ~PlayerPosMenu();

		virtual bool Attach() override;
		virtual void Detach() override;

		virtual void OnShow() override;
		virtual void OnHide() override;
	private:
		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;

		UiModule::VarTextEditableController<Game::SCR_f>* m_xController = nullptr;
		UiModule::VarTextEditableController<Game::SCR_f>* m_yController = nullptr;
		UiModule::VarTextEditableController<Game::SCR_f>* m_zController = nullptr;
	};
}