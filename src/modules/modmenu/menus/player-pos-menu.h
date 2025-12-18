#pragma once
#include "../common.h"
#include "../menu-base.h"
#include "../segment-support.h"

namespace ModMenuModule {
	class PositionSegment;

	class PlayerPosMenu : public MenuBase, public SegmentSupport {
	public:
		PlayerPosMenu();
		virtual ~PlayerPosMenu();

	private:
		virtual bool Attach() override;
		virtual void Detach() override;
		virtual void OnShow() override;
		virtual void OnHide() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;

		void Teleport();

		PositionSegment* m_positionSegment = nullptr;
	};
}
