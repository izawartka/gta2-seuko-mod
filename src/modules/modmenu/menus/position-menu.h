#pragma once
#include "../common.h"
#include "../menu-base.h"
#include "../segment-support.h"
#include "../segments/position-segment.h"
#include "../cheats/position-store.h"

namespace ModMenuModule {
	class PositionMenu : public MenuBase, public SegmentSupport {
	public:
		PositionMenu(PositionStoreCheat::PositionId positionId, std::string_view persistenceKey);
		virtual ~PositionMenu();

	private:
		virtual bool Attach() override;
		virtual void Detach() override;
		virtual void OnShow() override;
		virtual void OnHide() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;

		PositionSegment* m_posSegment = nullptr;
	};
}
