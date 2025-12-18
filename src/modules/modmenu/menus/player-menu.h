#pragma once
#include "../common.h"
#include "../menu-base.h"
#include "../events/cheat-state.h"

namespace ModMenuModule {
	class PlayerMenu : public MenuBase, public Core::EventListenerSupport {
	public:
		PlayerMenu();
		virtual ~PlayerMenu();

	private:
		virtual bool Attach() override;

		virtual void OnShow() override;
		virtual void OnHide() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
		void OnCheatStateChange(CheatStateEvent& event);
		void UpdateCheatStates();

		UiModule::SelectController<bool>* m_freezeCopValueController = nullptr;
		UiModule::SelectController<bool>* m_invulnerabilityController = nullptr;
	};
}
