#pragma once
#include "../common.h"
#include "../menu-base.h"
#include "../cheat-support.h"
#include "../events/cheat-state.h"

namespace ModMenuModule {
	class CameraPosMenu : public MenuBase, public Core::EventListenerSupport, public CheatSupport {
	public:
		CameraPosMenu();
		virtual ~CameraPosMenu();

		virtual bool Attach() override;
		virtual void Detach() override;
		virtual void OnShow() override;
		virtual void OnHide() override;

	private:
		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;

		void AttachCheatMenuItems();
		void DetachCheatMenuItems();
		void OnCheatStateChange(CheatStateEvent& event);
		void UpdateCheatStates();

		UiModule::VertCont* m_cheatItemsOuterCont = nullptr;
		UiModule::VertCont* m_cheatItemsInnerCont = nullptr;
		UiModule::SelectController<bool>* m_cameraPosCheatController = nullptr;
	};
}
