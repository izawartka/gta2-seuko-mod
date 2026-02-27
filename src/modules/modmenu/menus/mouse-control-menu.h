#pragma once
#include "../common.h"
#include "../menu-base.h"
#include "../events/cheat-state.h"
#include "../events/cheat-options-update.h"

namespace ModMenuModule {
	class MouseControlCheat;

	class MouseControlMenu : public MenuBase, public Core::EventListenerSupport {
	public:
		MouseControlMenu();
		virtual ~MouseControlMenu();

	private:
		virtual bool Attach() override;
		virtual void Detach() override;

		virtual void OnShow() override;
		virtual void OnHide() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
		void OnCheatStateChange(CheatStateEvent& event);
		void OnCheatOptionsUpdate(CheatOptionsUpdateEvent<MouseControlCheat>& event);

		void AttachCheatMenuItems();
		void DetachCheatMenuItems();
		void AttachRotateModeMenuItems();
		void DetachRotateModeMenuItems();
		void UpdateAttachedItems();
		void UpdateRotateModeMenuItemsAttached();

		UiModule::SelectController<bool>* m_mouseControlCheatController = nullptr;
		UiModule::VertCont* m_cheatItemsCont = nullptr;
		UiModule::MenuItemGroupId m_cheatItemsGroupId = -1;
		bool m_cheatItemsAttached = false;
		UiModule::VertCont* m_rotateModeItemsCont = nullptr;
		UiModule::MenuItemGroupId m_rotateModeItemsGroupId = -1;
		bool m_rotateModeItemsAttached = false;
	};
}
