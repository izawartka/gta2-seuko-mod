#pragma once
#include "../common.h"
#include "../menu-base.h"
#include "../events/last-car-state.h"

namespace ModMenuModule {
	class VehiclesMenu : public MenuBase, public Core::EventListenerSupport {
	public:
		VehiclesMenu();
		virtual ~VehiclesMenu();

	private:
		virtual bool Attach() override;

		virtual void OnShow() override;
		virtual void OnHide() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
		void OnLastCarStateChange(ModMenuModule::LastCarStateEvent& event);
		void OnCheatStateChange(CheatStateEvent& event);

		void UpdateLastCarState();
		void UpdateCheatStates();

		UiModule::Text* m_lastCarText = nullptr;
		UiModule::SelectController<bool>* m_disableSteeringAssistCheatController = nullptr;
	};
}
