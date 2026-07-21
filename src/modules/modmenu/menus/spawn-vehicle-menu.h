#pragma once
#include "../common.h"
#include "../menu-base.h"
#include "../segment-support.h"

namespace ModMenuModule {
	class PositionRotationMenuSegment;
	class SpawnVehicleSegment;

	class SpawnVehicleMenu : public MenuBase, public SegmentSupport {
	public:
		SpawnVehicleMenu();
		virtual ~SpawnVehicleMenu();

	private:
		virtual bool Attach() override;
		virtual void Detach() override;
		virtual void OnShow() override;
		virtual void OnHide() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
		void Spawn();

		PositionRotationMenuSegment* m_posRotMenuSegment = nullptr;
		SpawnVehicleSegment* m_spawnVehicleSegment = nullptr;
	};
}
