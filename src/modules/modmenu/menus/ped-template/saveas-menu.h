#pragma once
#include "../../common.h"
#include "../../menu-base.h"
#include "../../cheats/ped-templates.h"

namespace ModMenuModule {
	class PedTemplateSaveasMenu : public MenuBase {
	public:
		PedTemplateSaveasMenu(TempPedTemplateId tempTemplateId);
		virtual ~PedTemplateSaveasMenu();

	private:
		virtual bool Attach() override;
		virtual void Detach() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
		bool SaveTemplate();

		TempPedTemplateId m_tempTemplateId = -1;
		std::shared_ptr<PedTemplate> m_tempTemplatePtr = nullptr;
		UiModule::EditableController<std::wstring>* m_templateNameController = nullptr;
		bool m_overwriteConfirmed = false;
	};
}
