#pragma once
#include "../common.h"
#include "../menu-base.h"
#include "../events/cheat-state.h"

namespace ModMenuModule {
	class WeaponsMenu : public MenuBase, public Core::EventListenerSupport {
	public:
		WeaponsMenu();
		virtual ~WeaponsMenu();

	private:
		virtual bool Attach() override;
		virtual void Detach() override;

		virtual void OnShow() override;
		virtual void OnHide() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
		void OnCheatStateChange(CheatStateEvent& event);
		void UpdateCheatStates();

		UiModule::SelectController<bool>* m_infiniteAmmoController = nullptr;
		UiModule::SelectController<bool>* m_instantReloadController = nullptr;
		UiModule::VarTextController<Game::WEAPON_INDEX>* m_weaponController = nullptr;
	};
}
