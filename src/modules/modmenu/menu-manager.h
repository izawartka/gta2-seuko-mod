#pragma once
#include "common.h"
#include "menu-base.h"
#include "../../events/keyboard.h"

namespace ModMenuModule {
	class MenuManager : public Core::EventListenerSupport {
	public:
		static MenuManager* GetInstance();

		template<typename MenuT, typename... Args>
		MenuT* AddMenu(Args&&... args) {
			static_assert(std::is_base_of_v<MenuBase, MenuT>, "MenuT must be derived from MenuBase");

			MenuT* newMenu = new MenuT(std::forward<Args>(args)...);
			if (!newMenu->Attach()) {
				spdlog::error("Failed to attach mod menu {}", typeid(MenuT).name());
				delete newMenu;
				return nullptr;
			}
			if (m_visible) {
				newMenu->SetVisible(true);
			}

			if (!m_menus.empty()) {
				m_menus.back()->SetVisible(false);
				m_menus.back()->Detach();
			}

			m_menus.emplace_back(std::unique_ptr<MenuBase>(newMenu));
			return newMenu;
		}

		void RemoveLastMenu();
		void ClearMenus();
		void SetVisible(bool visible);

	private:
		friend class RootModule;
		MenuManager();
		virtual ~MenuManager();
		MenuManager(const MenuManager&) = delete;
		MenuManager& operator=(const MenuManager&) = delete;

		static MenuManager* m_instance;

		void OnKeyDown(KeyDownEvent& event);
		void Attach();
		void Detach();

		std::vector<std::unique_ptr<MenuBase>> m_menus;
		bool m_visible = false;
	};
}