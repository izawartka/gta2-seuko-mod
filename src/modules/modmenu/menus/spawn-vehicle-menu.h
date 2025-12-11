#pragma once
#include "../common.h"
#include "../menu-base.h"
#include "../segment-support.h"

namespace ModMenuModule {
	class SpawnVehicleSegment;

	class SpawnVehicleMenu : public MenuBase, public SegmentSupport {
	public:
		SpawnVehicleMenu();
		virtual ~SpawnVehicleMenu();

		virtual bool Attach() override;
		virtual void Detach() override;

	private:
		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
		void Spawn();

		SpawnVehicleSegment* m_spawnVehicleSegment = nullptr;
	};
}
