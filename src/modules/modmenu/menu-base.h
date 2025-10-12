#pragma once
#include "common.h"

namespace ModMenuModule {
	class MenuBase {
	public:
		virtual ~MenuBase() = default;
	protected:
		friend class RootModule;
		MenuBase() = default;

		virtual bool Attach();
		virtual void Detach();

		virtual void CreateMenu(std::wstring title, UiModule::Component*& vertCont) final;
		virtual void ApplyIndexPersistence(std::string key) final;
		virtual void DestroyMenu() final;

		virtual void OnShow() {}
		virtual void OnHide() {}

		virtual void SetVisible(bool visible) final;
		virtual bool IsVisible() const final { return m_visible; }

		virtual void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) = 0;

		UiModule::Component* m_mainComponent = nullptr;
		UiModule::MenuController* m_menuController = nullptr;
		bool m_visible = false;
		std::string m_selectedItemPersistenceKey = "";
	};
}
