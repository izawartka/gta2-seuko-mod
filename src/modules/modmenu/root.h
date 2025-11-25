#pragma once
#include "common.h"
#include "cheat-base.h"
#include "menu-manager.h"
#include "reset-binds-manager.h"
#include "quick-action-manager.h"

namespace ModMenuModule {
	struct ModMenuOptions {
		std::string keyBindToggleMenuName = "ModMenu_ToggleMenu";
		KeyBindingModule::Key keyBindToggleMenuDefault = KeyBindingModule::Key(Game::KeyCode::DIK_F11);
		unsigned int resetBindsHoldTime = 90; // frames
		Game::SCR_f menuX = Game::Utils::FromFloat(9.0f);
		Game::SCR_f menuY = Game::Utils::FromFloat(90.0f);
		Game::SCR_f menuHeaderMarginX = Game::Utils::FromFloat(15.0f);
		Game::SCR_f menuHeaderMarginY = Game::Utils::FromFloat(10.0f);
		Game::SCR_f menuHeaderTextSize = Game::Utils::FromFloat(1.0f);
		Game::SCR_f menuSpacerHeight = Game::Utils::FromFloat(5.0f);
		Game::SCR_f textSize = Game::Utils::FromFloat(1.0f);
		Game::SCR_f menuPadding = Game::Utils::FromFloat(5.0f);
		Game::SCR_f minMenuWidth = Game::Utils::FromFloat(250.0f);
		UiModule::MenuControllerOptions menuControllerOptions = {};
		UiModule::BackgroundOptions menuBackgroundOptions = { 128 };
	};

	class RootModule : public Core::ModuleBase {
	public:
		RootModule(ModMenuOptions options = {});
		virtual ~RootModule() override;
		static RootModule* GetInstance();
		bool Attach();
		void Detach();

		template<typename CheatT>
		CheatT* GetCheat() {
			static_assert(std::is_base_of_v<CheatBase, CheatT>, "CheatT must be derived from CheatBase");
			auto it = m_cheats.find(std::type_index(typeid(CheatT)));
			if (it == m_cheats.end()) {
				spdlog::error("Requested cheat {} is not registered", typeid(CheatT).name());
				return nullptr;
			}
			return static_cast<CheatT*>(it->second.get());
		}

		ModMenuOptions GetOptions() const { return m_options; }

	private:
		void InstantiateCheats();
		void DestroyCheats();
		static RootModule* m_instance;

		ModMenuOptions m_options;
		MenuManager m_menuManager;
		ResetBindsManager m_resetBindsManager;
		QuickActionManager m_quickActionManager;
		std::unordered_map<std::type_index, std::unique_ptr<CheatBase>> m_cheats;
	};
}
