#pragma once
#include "common.h"
#include "segment-base.h"

namespace ModMenuModule {
	class MenuBase {
	public:
		virtual ~MenuBase();

	protected:
		friend class MenuManager;
		friend class SegmentBase;
		MenuBase() = default;

		virtual bool Attach();
		virtual void Detach();

		virtual void SetAttached(bool attached) final;
		virtual bool IsAttached() const final { return m_attached; }

		virtual void CreateMenu(std::wstring title, UiModule::Component*& vertCont) final;
		virtual void SetPreviousSelectedIndex() final;
		virtual void DestroyMenu() final;
		void SetTitle(std::wstring title);

		virtual void OnShow() {}
		virtual void OnHide() {}

		virtual void SetVisible(bool visible) final;
		virtual bool IsVisible() const final { return m_visible; }

		virtual void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) = 0;

		UiModule::Text* m_titleText = nullptr;
		UiModule::Component* m_mainComponent = nullptr;
		UiModule::MenuController* m_menuController = nullptr;
		bool m_attached = false;
		bool m_visible = false;
		size_t m_selectedIndex = -1;
	};
}
