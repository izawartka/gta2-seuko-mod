#pragma once
#include "../common.h"
#include "../menu-base.h"
#include "../segment-support.h"

namespace ModMenuModule {
	class GetPowerupSegment;

	class GetPowerupMenu : public MenuBase, public SegmentSupport {
	public:
		GetPowerupMenu();
		virtual ~GetPowerupMenu();

	private:
		virtual bool Attach() override;
		virtual void Detach() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
		void GetPowerup();

		GetPowerupSegment* m_getPowerupSegment = nullptr;
	};
}
