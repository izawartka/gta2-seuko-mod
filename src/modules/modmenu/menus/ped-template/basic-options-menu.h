#pragma once
#include "../../common.h"
#include "../../menu-base.h"
#include "../../cheats/ped-templates.h"
#include "../../segment-support.h"

namespace ModMenuModule {
	class PedPreviewSegment;

	class PedTemplateBasicOptionsMenu : public MenuBase, public SegmentSupport {
	public:
		PedTemplateBasicOptionsMenu(TempPedTemplateId tempTemplateId);
		virtual ~PedTemplateBasicOptionsMenu();

	private:
		virtual bool Attach() override;
		virtual void Detach() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
		void UpdatePreview();

		TempPedTemplateId m_tempTemplateId = -1;
		std::shared_ptr<PedTemplate> m_tempTemplatePtr = nullptr;
		PedPreviewSegment* m_pedPreviewSegment = nullptr;
	};
}
