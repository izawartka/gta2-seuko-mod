#pragma once
#include "../common.h"
#include "../events/player-appearance-update.h"
#include "../menu-base.h"

namespace ModMenuModule {
	class PlayerAppearanceMenu : public MenuBase, public Core::EventListenerSupport {
	public:
		PlayerAppearanceMenu();
		virtual ~PlayerAppearanceMenu();

	private:
		virtual bool Attach() override;

		virtual void OnShow() override;
		virtual void OnHide() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
		void OnPlayerAppearanceUpdate(ModMenuModule::PlayerAppearanceUpdateEvent& event);
		void ForceUpdateControllers();

		UiModule::SelectController<Game::PED_GRAPHIC_TYPE>* m_graphicTypeController = nullptr;
		UiModule::SelectController<Game::PED_REMAP>* m_remapController = nullptr;
	};
}
