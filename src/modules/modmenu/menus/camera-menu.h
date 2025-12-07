#pragma once
#include "../common.h"
#include "../menu-base.h"
#include "../cheat-support.h"
#include "../events/cheat-state.h"

namespace ModMenuModule {
	class CameraMenu : public MenuBase, public Core::EventListenerSupport, public CheatSupport {
	public:
		CameraMenu();
		virtual ~CameraMenu();

		virtual bool Attach() override;
		virtual void OnShow() override;
		virtual void OnHide() override;

	private:
		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
		void OnCheatStateChange(CheatStateEvent& event);
		void UpdateCheatStates();

		UiModule::SelectController<bool>* m_cameraCheatController = nullptr;
		UiModule::SelectController<bool>* m_clearScreenCheatController = nullptr;
		UiModule::SelectController<bool>* m_disableCullingCheatController = nullptr;
	};
}
