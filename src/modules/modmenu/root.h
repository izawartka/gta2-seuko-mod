#pragma once
#include "common.h"
#include "menu-base.h"
#include "../../events/keyboard.h"

namespace ModMenuModule {
	struct ModMenuOptions {
		Game::KeyCode toggleKey = Game::KeyCode::DIK_F11;
		Game::SCR_f menuX = 0x40000;
		Game::SCR_f menuY = 0x200000;
		Game::SCR_f menuHeaderMarginX = 0x60000;
		Game::SCR_f menuHeaderMarginY = 0x30000;
		Game::SCR_f menuHeaderTextSize = 0x6000;
		Game::SCR_f textSize = 0x6000;
		UiModule::MenuControllerOptions menuControllerOptions = {};
	};

	class RootModule : public Core::ModuleBase {
	public:
		RootModule(ModMenuOptions options = {});
		virtual ~RootModule() override;
		static RootModule* GetInstance();
		bool Attach();
		void Detach();

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
				m_menus.back()->Detach();
			}

			m_menus.emplace_back(std::unique_ptr<MenuBase>(newMenu));
			return newMenu;
		}

		void RemoveLastMenu();
		void ClearMenus();
		void SetVisible(bool visible);
		ModMenuOptions GetOptions() const { return m_options; }

	private:
		void OnKeyDown(const KeyDownEvent& event);
		static RootModule* m_instance;

		ModMenuOptions m_options;
		std::vector<std::unique_ptr<MenuBase>> m_menus;
		bool m_visible = false;
	};
}
