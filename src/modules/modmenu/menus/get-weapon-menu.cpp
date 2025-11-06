#include "get-weapon-menu.h"
#include "../../../converters/weapon.h"
#include "../../../converters/ammo.h"
#include "../root.h"

ModMenuModule::GetWeaponMenu::GetWeaponMenu()
{

}

ModMenuModule::GetWeaponMenu::~GetWeaponMenu()
{

}

bool ModMenuModule::GetWeaponMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Get weapon#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	ModMenuModule::ModMenuOptions options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);

	// weapon
	UiModule::Text* weaponText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	UiModule::SelectOptionList<Game::WEAPON_INDEX> weaponOptionList = Game::Utils::GetAvailableWeapons();
	Game::WEAPON_INDEX weaponDefault = persistence->Load("ModMenu_GetWeaponMenu_SelectedWeapon", weaponOptionList[0]);
	m_weaponController = m_menuController->CreateLatestItemController<UiModule::SelectController<Game::WEAPON_INDEX>>(
		weaponText,
		weaponOptionList,
		weaponDefault,
		UiModule::SelectControllerOptions{ L"Weapon: #", L"#" }
	);
	m_weaponController->SetConverter<WeaponConverter>();

	// ammo
	UiModule::Text* ammoText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	short ammoDefault = persistence->Load("ModMenu_GetWeaponMenu_SelectedAmmo", (short)99);
	m_ammoController = m_menuController->CreateLatestItemController<UiModule::EditableController<short>>(
		ammoText,
		ammoDefault,
		UiModule::EditableControllerOptions{ L"Ammo: #", L"#" }
	);

	// get weapon button
	auto getWeaponText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"Get weapon", options.textSize);
	auto getWeaponBtn = m_menuController->CreateLatestItemController<UiModule::ButtonController>(getWeaponText);
	getWeaponBtn->SetCallback(this, &GetWeaponMenu::GetWeapon);

	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::GetWeaponMenu::Detach()
{
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();
	persistence->Save("ModMenu_GetWeaponMenu_SelectedWeapon", m_weaponController->GetValue().value());
	persistence->Save("ModMenu_GetWeaponMenu_SelectedAmmo", m_ammoController->GetValue().value());

	DestroyMenu();
}

void ModMenuModule::GetWeaponMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	switch (id) {
	case 0: // Go back
		ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
		break;
	default:
		break;
	}
}

void ModMenuModule::GetWeaponMenu::GetWeapon()
{
	Game::WEAPON_INDEX selectedWeapon = m_weaponController->GetValue().value();
	short selectedAmmo = m_ammoController->GetValue().value();

	Game::Ped* playerPed = Game::Memory::GetPlayerPed();
	if (!playerPed) {
		spdlog::error("Could not give weapon: player ped is null.");
		return;
	}

	if(Game::Utils::IsWeaponVehicleWeapon(selectedWeapon) && playerPed->currentCar == nullptr) {
		spdlog::warn("Could not give weapon: player is not in a vehicle to receive vehicle weapon.");
		return;
	}

	spdlog::info("Giving weapon #{} with {} ammo to player.", static_cast<int>(selectedWeapon), selectedAmmo);

	Game::Functions::AddWeapon(
		playerPed,
		0,
		selectedWeapon,
		selectedAmmo
	);

	Game::CarRoofUtils::AddCarRoofForWeapon(playerPed->currentCar, selectedWeapon);

	playerPed->playerWeapons->selectedWeapon = static_cast<short>(selectedWeapon);
}
