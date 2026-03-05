#pragma once
#include "../common.h"
#include "../menu-base.h"
#include "../segment-support.h"
#include "../quick-action-manager.h"

namespace ModMenuModule {
	class QuickActionKeySegment;

	class QuickActionOptionsMenu : public MenuBase, public SegmentSupport {
	public:
		QuickActionOptionsMenu(QuickActionId actionId);
		virtual ~QuickActionOptionsMenu();

	private:
		virtual bool Attach() override;
		virtual void Detach() override;

		virtual void OnShow() override;
		virtual void OnHide() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
		bool SaveAction();

		QuickActionKeySegment* m_keySegment = nullptr;
		QuickActionId m_actionId;
		std::optional<QuickActionInfo> m_actionInfo = std::nullopt;
		UiModule::MenuItemId m_saveBtnMenuId = 0;
		UiModule::MenuItemId m_deleteBtnMenuId = 0;
	};
}
