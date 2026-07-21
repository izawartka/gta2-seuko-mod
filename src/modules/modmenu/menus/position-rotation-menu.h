#pragma once
#include "../common.h"
#include "../menu-base.h"
#include "../segment-support.h"
#include "../segments/position-rotation-segment.h"
#include "../cheats/position-store.h"

namespace ModMenuModule {
	class PositionRotationMenu : public MenuBase, public SegmentSupport {
	public:
		PositionRotationMenu(PositionStoreCheat::PositionId positionId, std::string_view persistenceKey);
		virtual ~PositionRotationMenu();

	private:
		virtual bool Attach() override;
		virtual void Detach() override;
		virtual void OnShow() override;
		virtual void OnHide() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;

		PositionRotationSegment* m_posRotSegment = nullptr;
	};
}
