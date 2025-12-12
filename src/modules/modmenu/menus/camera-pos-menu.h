#pragma once
#include "../common.h"
#include "../menu-base.h"
#include "../segment-support.h"
#include "../cheat-support.h"
#include "../events/cheat-state.h"

namespace ModMenuModule {
	class CameraPosMenu : public MenuBase, public Core::EventListenerSupport, public SegmentSupport, public CheatSupport {
	public:
		CameraPosMenu();
		virtual ~CameraPosMenu();

	private:
		virtual bool Attach() override;
		virtual void Detach() override;

		virtual void OnShow() override;
		virtual void OnHide() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;

		void AttachCheatMenuItems();
		void DetachCheatMenuItems();
		void OnCheatStateChange(CheatStateEvent& event);
		void UpdateCheatStates();

		UiModule::VertCont* m_cheatItemsCont = nullptr;
		UiModule::SelectController<bool>* m_cameraPosCheatController = nullptr;
		UiModule::MenuItemGroupId m_cheatItemsGroupId = -1;
		UiModule::Spacer* m_cheatItemsSpacer = nullptr;
		bool m_cheatItemsAttached = false;
	};
}
