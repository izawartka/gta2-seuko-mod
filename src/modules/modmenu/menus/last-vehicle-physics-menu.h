#pragma once
#include "../common.h"
#include "../menu-base.h"
#include "../cheat-support.h"
#include "../events/last-car-state.h"

namespace ModMenuModule {
	class LastVehiclePhysicsMenu : public MenuBase, public Core::EventListenerSupport, public CheatSupport {
	public:
		LastVehiclePhysicsMenu();
		virtual ~LastVehiclePhysicsMenu();

		virtual bool Attach() override;

		virtual void OnShow() override;
		virtual void OnHide() override;

	private:
		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
		void OnLastCarStateChange(ModMenuModule::LastCarStateEvent& event);
		void UpdateLastCarState();
		Game::Car* GetLastCar();
	};
}
