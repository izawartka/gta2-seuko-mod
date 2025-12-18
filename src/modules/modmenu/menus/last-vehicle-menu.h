#pragma once
#include "../common.h"
#include "../menu-base.h"
#include "../events/last-car-state.h"

namespace ModMenuModule {
	class LastVehicleMenu : public MenuBase, public Core::EventListenerSupport {
	public:
		LastVehicleMenu();
		virtual ~LastVehicleMenu();

	private:
		virtual bool Attach() override;
		virtual void Detach() override;

		virtual void OnShow() override;
		virtual void OnHide() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;

		void OnLastCarStateChange(ModMenuModule::LastCarStateEvent& event);
		void UpdateLastCarState();
		Game::Car* GetLastCar();
		void FixCarDamage();
		void TurnEngineOff();
		void ExplodeCar();

		UiModule::VarTextController<Game::CAR_MODEL4>* m_modelController = nullptr;
	};
}
