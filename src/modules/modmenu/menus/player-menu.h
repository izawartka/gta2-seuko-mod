#pragma once
#include "../common.h"
#include "../cheat-support.h"
#include "../menu-base.h"

namespace ModMenuModule {
	class PlayerMenu : public MenuBase, public CheatSupport, public Core::EventListenerSupport {
	public:
		PlayerMenu();
		virtual ~PlayerMenu();

		virtual bool Attach() override;
		virtual void Detach() override;

		virtual void OnShow() override;
		virtual void OnHide() override;

	private:
		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
		void OnCheatStateChange(CheatStateEvent& event);
		void UpdateCheatStates();

		UiModule::VarTextSelectController<short>* m_wantedLevelController = nullptr;
		UiModule::SelectController<bool>* m_freezeCopValueController = nullptr;
		UiModule::VarTextEditableController<Game::ushort>* m_healthController = nullptr;
		UiModule::SelectController<bool>* m_invulnerabilityController = nullptr;
	};
}
