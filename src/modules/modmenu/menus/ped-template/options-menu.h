#pragma once
#include "../../common.h"
#include "../../menu-base.h"
#include "../../cheats/ped-templates.h"

namespace ModMenuModule {
	class PedTemplateOptionsMenu : public MenuBase {
	public:
		PedTemplateOptionsMenu(TempPedTemplateId tempTemplateId);
		virtual ~PedTemplateOptionsMenu();

	private:
		virtual bool Attach() override;
		virtual void Detach() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
		void SaveTemplate();
		void DeleteTemplate();
		void UpdateSaveItems();

		TempPedTemplateId m_tempTemplateId = -1;
		std::shared_ptr<PedTemplate> m_tempTemplatePtr = nullptr;
		UiModule::VertCont* m_saveItemsCont = nullptr;
		UiModule::MenuItemGroupId m_saveItemsGroupId = -1;
		size_t m_saveItemsIndex = 0;
		UiModule::MenuItemId m_saveBtnMenuItemId = -1;
		UiModule::MenuItemId m_saveAsNewBtnMenuItemId = -1;
		UiModule::MenuItemId m_deleteBtnMenuItemId = -1;
	};
}
