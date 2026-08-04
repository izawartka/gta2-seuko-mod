#pragma once
#include "../../common.h"
#include "../../menu-base.h"
#include "../../cheats/ped-templates.h"
#include "../../segment-support.h"

namespace ModMenuModule {
	class EncodedPedIdSegment;

	class PedTemplateLogicOptionsMenu : public MenuBase, public SegmentSupport {
	public:
		PedTemplateLogicOptionsMenu(TempPedTemplateId tempTemplateId);
		virtual ~PedTemplateLogicOptionsMenu();

	private:
		virtual bool Attach() override;
		virtual void Detach() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;

		TempPedTemplateId m_tempTemplateId = -1;
		EncodedPedIdSegment* m_leaderIdSegment = nullptr;
		std::shared_ptr<PedTemplate> m_tempTemplatePtr = nullptr;
	};
}
