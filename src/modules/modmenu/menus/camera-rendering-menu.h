#pragma once
#include "../common.h"
#include "../menu-base.h"
#include "../events/cheat-state.h"

namespace ModMenuModule {
	class CameraRenderingMenu : public MenuBase, public Core::EventListenerSupport {
	public:
		CameraRenderingMenu();
		virtual ~CameraRenderingMenu();

	private:
		virtual bool Attach() override;

		virtual void OnShow() override;
		virtual void OnHide() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;

		void OnCheatStateChange(CheatStateEvent& event);
		void UpdateCheatStates();

		UiModule::SelectController<bool>* m_clearScreenCheatController = nullptr;
		UiModule::SelectController<bool>* m_disableCullingCheatController = nullptr;
		UiModule::SelectController<bool>* m_shadowsFixCheatController = nullptr;
	};
}
