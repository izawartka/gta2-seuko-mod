#pragma once
#include "../../common.h"
#include "../../menu-base.h"
#include "../../cheats/ped-templates.h"

namespace ModMenuModule {
	class PedTemplateMiscOptionsMenu : public MenuBase {
	public:
		PedTemplateMiscOptionsMenu(TempPedTemplateId tempTemplateId);
		virtual ~PedTemplateMiscOptionsMenu();

	private:
		virtual bool Attach() override;
		virtual void Detach() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;

		TempPedTemplateId m_tempTemplateId = -1;
		std::shared_ptr<PedTemplate> m_tempTemplatePtr = nullptr;
	};
}
