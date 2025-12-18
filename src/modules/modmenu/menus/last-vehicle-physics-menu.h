#pragma once
#include "../common.h"
#include "../menu-base.h"
#include "../events/last-car-state.h"

namespace ModMenuModule {
	class LastVehiclePhysicsMenu : public MenuBase, public Core::EventListenerSupport {
	public:
		LastVehiclePhysicsMenu();
		virtual ~LastVehiclePhysicsMenu();

	private:
		virtual bool Attach() override;

		virtual void OnShow() override;
		virtual void OnHide() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
		void OnLastCarStateChange(ModMenuModule::LastCarStateEvent& event);
		void UpdateLastCarState();
		Game::Car* GetLastCar();
	};
}
