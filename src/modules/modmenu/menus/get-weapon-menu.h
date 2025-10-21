#pragma once
#include "../common.h"
#include "../menu-base.h"

namespace ModMenuModule {
	class GetWeaponMenu : public MenuBase {
	public:
		GetWeaponMenu();
		virtual ~GetWeaponMenu();

		virtual bool Attach() override;
		virtual void Detach() override;

		virtual void OnShow() override;
		virtual void OnHide() override;

	private:
		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;
		void GetWeapon();

		UiModule::SelectController<Game::WEAPON_INDEX>* m_weaponController = nullptr;
		UiModule::VarTextEditableController<short>* m_ammoController = nullptr;
		short m_selectedAmmo = 99;
	};
}
