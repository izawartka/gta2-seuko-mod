#pragma once
#include "../common.h"
#include "../menu-base.h"
#include "../events/cheat-state.h"

namespace ModMenuModule {
	class SmoothCameraMenu : public MenuBase, public Core::EventListenerSupport {
	public:
		SmoothCameraMenu();
		virtual ~SmoothCameraMenu();

	private:
		virtual bool Attach() override;
		virtual void Detach() override;

		virtual void OnShow() override;
		virtual void OnHide() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;

		void AttachCheatMenuItems();
		void DetachCheatMenuItems();
		void OnCheatStateChange(CheatStateEvent& event);
		void UpdateCheatState();

		UiModule::SelectController<bool>* m_smoothCameraCheatController = nullptr;
		UiModule::VertCont* m_cheatItemsCont = nullptr;
		UiModule::MenuItemGroupId m_cheatItemsGroupId = -1;
		bool m_cheatItemsAttached = false;
		UiModule::MenuItemId m_carCameraMenuItemId = -1;
		UiModule::MenuItemId m_followCameraMenuItemId = -1;
	};
}
