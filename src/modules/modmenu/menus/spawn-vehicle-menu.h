#pragma once
#include "../common.h"
#include "../menu-base.h"
#include "../segments/spawn-vehicle-segment.h"

namespace ModMenuModule {
	class SpawnVehicleMenu : public MenuBase {
	public:
		SpawnVehicleMenu();
		virtual ~SpawnVehicleMenu();

		virtual bool Attach() override;
		virtual void Detach() override;

	private:
		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
		void Spawn();

		SpawnVehicleSegment m_spawnSegment;
	};
}
