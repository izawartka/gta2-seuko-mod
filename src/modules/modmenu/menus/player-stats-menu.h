#pragma once
#include "../common.h"
#include "../menu-base.h"

namespace ModMenuModule {
	class PlayerStatsMenu : public MenuBase {
	public:
		PlayerStatsMenu();
		virtual ~PlayerStatsMenu();

		virtual bool Attach() override;
		virtual void Detach() override;

		virtual void OnShow() override;
		virtual void OnHide() override;
	private:
		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;

		UiModule::VarTextEditableController<Game::uint>* m_moneyController = nullptr;
		UiModule::VarTextEditableController<Game::uint>* m_multiplierController = nullptr;
		UiModule::VarTextEditableController<Game::uint>* m_livesController = nullptr;
		UiModule::VarTextEditableController<char>* m_respect1Controller = nullptr;
		UiModule::VarTextEditableController<char>* m_respect2Controller = nullptr;
		UiModule::VarTextEditableController<char>* m_respect3Controller = nullptr;
	};
}
