#pragma once
#include "../common.h"
#include "../menu-base.h"
#include "../segment-support.h"

namespace ModMenuModule {
	class PositionMenuSegment;
	class SpawnObjectSegment;

	class SpawnObjectMenu : public MenuBase, public SegmentSupport {
	public:
		SpawnObjectMenu();
		virtual ~SpawnObjectMenu();

	private:
		virtual bool Attach() override;
		virtual void Detach() override;
		virtual void OnShow() override;
		virtual void OnHide() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
		void Spawn();

		PositionMenuSegment* m_positionMenuSegment = nullptr;
		SpawnObjectSegment* m_spawnObjectSegment = nullptr;
	};
}
