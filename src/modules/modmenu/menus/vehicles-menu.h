#pragma once
#include "../common.h"
#include "../menu-base.h"
#include "../cheat-support.h"
#include "../events/last-car-state.h"

namespace ModMenuModule {
	class VehiclesMenu : public MenuBase, public Core::EventListenerSupport, public CheatSupport {
	public:
		VehiclesMenu();
		virtual ~VehiclesMenu();

	private:
		virtual bool Attach() override;

		virtual void OnShow() override;
		virtual void OnHide() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
		void OnLastCarStateChange(ModMenuModule::LastCarStateEvent& event);
		void UpdateLastCarState();

		UiModule::Text* m_lastCarText = nullptr;
	};
}
