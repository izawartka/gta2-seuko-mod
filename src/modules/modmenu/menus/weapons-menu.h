#pragma once
#include "../common.h"
#include "../menu-base.h"

static constexpr Game::SCR_f CURRENT_WEAPON_OFFSET_Y = Game::Utils::FromFloat(12.0f);

namespace ModMenuModule {
	class WeaponsMenu : public MenuBase {
	public:
		WeaponsMenu();
		virtual ~WeaponsMenu();

		virtual bool Attach() override;
		virtual void Detach() override;

		virtual void OnShow() override;
		virtual void OnHide() override;

	private:
		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;

		UiModule::VarTextController<Game::WEAPON_INDEX>* m_weaponController = nullptr;
		UiModule::VarTextEditableController<short>* m_ammoController = nullptr;
	};
}
