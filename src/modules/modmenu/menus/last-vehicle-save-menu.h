#pragma once
#include "../common.h"
#include "../menu-base.h"
#include "../events/last-car-state.h"

namespace ModMenuModule {
	class LastVehicleSaveMenu : public MenuBase, public Core::EventListenerSupport {
	public:
		LastVehicleSaveMenu();
		virtual ~LastVehicleSaveMenu();

	private:
		virtual bool Attach() override;

		virtual void OnShow() override;
		virtual void OnHide() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
		void OnLastCarStateChange(ModMenuModule::LastCarStateEvent& event);
		void UpdateLastCarState();
		bool SaveCar();

		UiModule::EditableController<std::wstring>* m_nameController = nullptr;
	};
}
