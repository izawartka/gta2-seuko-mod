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
	ModMenuModule::PersistenceManager* persistence = ModMenuModule::PersistenceManager::GetInstance();

	auto onEditStop = [this]() {
		if (m_visible) m_menuController->SetActive(true);
	};

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);

	// weapon
	UiModule::Text* weaponText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	UiModule::VarTextSelectOptionList<Game::WEAPON_INDEX> weaponOptionList = Game::Utils::GetAvailableWeapons();
	Game::WEAPON_INDEX selectedWeapon = persistence->Load("ModMenu_GetWeaponMenu_SelectedWeapon", weaponOptionList[0]);
	m_weaponController = uiRoot->AddController<UiModule::SelectController<Game::WEAPON_INDEX>>(
		weaponText,
		weaponOptionList,
		selectedWeapon,
		UiModule::SelectControllerOptions{ L"Weapon: #", L"#" }
	);
	m_weaponController->SetConverter<WeaponConverter>();
	m_weaponController->SetEditStopCallback(onEditStop);

	// ammo
	UiModule::Text* ammoText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_selectedAmmo = persistence->Load("ModMenu_GetWeaponMenu_SelectedAmmo", (short)99);
	m_ammoController = uiRoot->AddController<UiModule::VarTextEditableController<short>>(
		ammoText,
		[this]() { return &m_selectedAmmo; },
		UiModule::VarTextEditableControllerOptions{ L"Ammo: #", L"#" }
	);
	m_ammoController->SetEditStopCallback(onEditStop);

	// get weapon button
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Get weapon", options.textSize);

	ApplyIndexPersistence("ModMenu_GetWeaponMenu_SelectedIndex");

	return true;
}

void ModMenuModule::GetWeaponMenu::Detach()
{
	ModMenuModule::PersistenceManager* persistence = ModMenuModule::PersistenceManager::GetInstance();
	persistence->Save("ModMenu_GetWeaponMenu_SelectedWeapon", m_weaponController->GetValue());
	persistence->Save("ModMenu_GetWeaponMenu_SelectedAmmo", m_selectedAmmo);

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	uiRoot->RemoveController(m_weaponController);
	uiRoot->RemoveController(m_ammoController);
	DestroyMenu();
}

void ModMenuModule::GetWeaponMenu::OnShow()
{
	m_ammoController->SetWatching(true);
}

void ModMenuModule::GetWeaponMenu::OnHide()
{
	m_weaponController->SetEditing(false);
	m_ammoController->SetWatching(false);
	m_ammoController->SetEditing(false);
}

void ModMenuModule::GetWeaponMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	switch (id) {
	case 0: // Go back
		ModMenuModule::RootModule::GetInstance()->RemoveLastMenu();
		break;
	case 1: // Weapon
		m_menuController->SetActive(false);
		m_weaponController->SetEditing(true);
		break;
	case 2: // Ammo
		m_menuController->SetActive(false);
		m_ammoController->SetEditing(true);
		break;
	case 3: // Get weapon
		GetWeapon();
		break;
	default:
		break;
	}
}

void ModMenuModule::GetWeaponMenu::GetWeapon()
{
	Game::WEAPON_INDEX selectedWeapon = m_weaponController->GetValue().value();

	Game::Ped* playerPed = Game::Memory::GetPlayerPed();
	if (!playerPed) {
		spdlog::error("Could not give weapon: player ped is null.");
		return;
	}

	if(Game::Utils::IsWeaponVehicleWeapon(selectedWeapon) && playerPed->currentCar == nullptr) {
		spdlog::warn("Could not give weapon: player is not in a vehicle to receive vehicle weapon.");
		return;
	}

	spdlog::info("Giving weapon #{} with {} ammo to player.", static_cast<int>(selectedWeapon), m_selectedAmmo);

	Game::Functions::AddWeapon(
		playerPed,
		0,
		selectedWeapon,
		m_selectedAmmo
	);

	Game::CarRoofUtils::AddCarRoofForWeapon(playerPed->currentCar, selectedWeapon);

	playerPed->playerWeapons->selectedWeapon = static_cast<short>(selectedWeapon);
}
