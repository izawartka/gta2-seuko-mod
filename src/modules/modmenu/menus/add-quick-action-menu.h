#pragma once
#include "../common.h"
#include "../menu-base.h"
#include "../segment-support.h"
#include "../quick-action-registry.h"

namespace ModMenuModule {
	class QuickActionKeySegment;

	class AddQuickActionMenu : public MenuBase, public SegmentSupport {
	public:
		AddQuickActionMenu();
		virtual ~AddQuickActionMenu();

	private:
		virtual bool Attach() override;
		virtual void Detach() override;

		virtual void OnShow() override;
		virtual void OnHide() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
		void OnSave();

		void OnActionTypeChange(QuickActionTypeIndex actionType);
		void CreateOptionsSegment(QuickActionTypeIndex actionType);
		void AttachOptionsSegment();
		void DetachOptionsSegment();
		void DestroyOptionsSegment();

		QuickActionKeySegment* m_keySegment = nullptr;
		UiModule::SelectController<QuickActionTypeIndex>* m_actionTypeController = nullptr;
		UiModule::MenuItemId m_saveBtnMenuId = 0;
		size_t m_optionsSegmentBaseIndex = -1;
		UiModule::VertCont* m_optionsSegmentContainer = nullptr;
		SegmentBase* m_optionsSegment = nullptr;

		// semi-persistence for segments that use submenus
		std::optional<KeyBindingModule::Key> m_lastSelectedKey = std::nullopt;
		std::optional<QuickActionTypeIndex> m_lastSelectedActionType = std::nullopt;
	};
}
