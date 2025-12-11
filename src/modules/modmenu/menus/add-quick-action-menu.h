#pragma once
#include "../common.h"
#include "../menu-base.h"
#include "../segment-support.h"
#include "../quick-action-registry.h"

namespace ModMenuModule {
	class AddQuickActionMenu : public MenuBase, public SegmentSupport {
	public:
		AddQuickActionMenu();
		virtual ~AddQuickActionMenu();

		virtual bool Attach() override;
		virtual void Detach() override;

		virtual void OnShow() override;
		virtual void OnHide() override;

	private:
		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
		void OnSave();

		void OnActionTypeChange(QuickActionTypeIndex actionType);
		void CreateSegment(QuickActionTypeIndex actionType);
		void DestroySegment();

		UiModule::KeyChangeController* m_keyController = nullptr;
		UiModule::SelectController<QuickActionTypeIndex>* m_actionTypeController = nullptr;
		UiModule::MenuItemId m_saveBtnMenuId = 0;
		size_t m_segmentBaseIndex = -1;
		UiModule::VertCont* m_segmentContainer = nullptr;
		SegmentBase* m_segmentInstance = nullptr;
	};
}
