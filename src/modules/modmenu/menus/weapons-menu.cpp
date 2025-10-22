#include "weapons-menu.h"
#include "get-weapon-menu.h"
#include "../cheats/infinite-ammo.h"
#include "../../../converters/yes-no.h"
#include "../../../converters/weapon.h"
#include "../../../converters/ammo.h"
#include "../root.h"

ModMenuModule::WeaponsMenu::WeaponsMenu()
{

}

ModMenuModule::WeaponsMenu::~WeaponsMenu()
{

}

bool ModMenuModule::WeaponsMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Weapons#", vertCont);
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	ModMenuModule::ModMenuOptions options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Get weapon", options.textSize);
	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Get all weapons", options.textSize);

	// infinite ammo
	UiModule::Text* infiniteAmmoText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_infiniteAmmoController = m_menuController->CreateLatestItemController<UiModule::SelectController<bool>>(
		infiniteAmmoText,
		UiModule::SelectOptionList<bool>{ false, true },
		std::nullopt,
		UiModule::SelectControllerOptions{ L"Infinite ammo: #", L"#" }
	);
	m_infiniteAmmoController->SetConverter<YesNoConverter>();
	m_infiniteAmmoController->SetSaveCallback([this](bool newValue) {
		SetCheatEnabled<ModMenuModule::InfiniteAmmoCheat>(newValue);
	});

	// current weapon
	Core::Resolver<Game::WEAPON_INDEX> selectedWeaponResolver = Core::MakeResolver(
		Game::Memory::GetPlayerPed,
		mem(&Game::Ped::selectedWeapon),
		mem(&Game::WEAPON_STRUCT::id)
	);

	UiModule::Spacer* weaponSpacer = uiRoot->AddComponent<UiModule::Spacer>(vertCont, 0, CURRENT_WEAPON_OFFSET_Y);
	UiModule::Margin* weaponTextMargin = uiRoot->AddComponent<UiModule::Margin>(vertCont, options.menuControllerOptions.createdSelectableOptions.markerOffsetX, 0);
	UiModule::Text* weaponText = uiRoot->AddComponent<UiModule::Text>(weaponTextMargin, L"", options.textSize);
	m_weaponController = uiRoot->AddController<UiModule::VarTextController<Game::WEAPON_INDEX>>(
		weaponText,
		selectedWeaponResolver,
		UiModule::VarTextControllerOptions{ L"Weapon: #", L"#", L"None" }
	);
	m_weaponController->SetConverter<WeaponConverter>();

	// ammo
	Core::Resolver<short> ammoResolver = Core::MakeResolver(
		Game::Memory::GetPlayerPed,
		mem(&Game::Ped::selectedWeapon),
		mem(&Game::WEAPON_STRUCT::ammo)
	);

	UiModule::Text* ammoValueText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	auto ammoController = m_menuController->CreateLatestItemController<UiModule::VarTextEditableController<short>>(
		ammoValueText,
		ammoResolver,
		UiModule::VarTextEditableControllerOptions{ L"Ammo: #", L"#" }
	);
	ammoController->SetConverter<AmmoConverter>();

	ApplyIndexPersistence("ModMenu_WeaponsMenu_SelectedIndex");

	return true;
}

void ModMenuModule::WeaponsMenu::Detach()
{
	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	uiRoot->RemoveController(m_weaponController);

	DestroyMenu();
}

void ModMenuModule::WeaponsMenu::OnShow()
{
	m_weaponController->SetWatching(true);

	AddEventListener<ModMenuModule::CheatStateEvent>(&WeaponsMenu::OnCheatStateChange);
	UpdateCheatStates();
}

void ModMenuModule::WeaponsMenu::OnHide()
{
	RemoveEventListener<ModMenuModule::CheatStateEvent>();

	m_weaponController->SetWatching(false);
}

void ModMenuModule::WeaponsMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	switch (id) {
	case 0: // Go back
		ModMenuModule::RootModule::GetInstance()->RemoveLastMenu();
		break;
	case 1: // Get weapon
		ModMenuModule::RootModule::GetInstance()->AddMenu<ModMenuModule::GetWeaponMenu>();
		break;
	case 2: // Get all weapons
		GetAllWeapons();
		break;
	default:
		break;
	}
}

void ModMenuModule::WeaponsMenu::OnCheatStateChange(CheatStateEvent& event)
{
	if (event.GetCheatType() == typeid(ModMenuModule::InfiniteAmmoCheat)) {
		m_infiniteAmmoController->SetValue(event.IsEnabled());
	}
}

void ModMenuModule::WeaponsMenu::UpdateCheatStates()
{
	m_infiniteAmmoController->SetValue(IsCheatEnabled<ModMenuModule::InfiniteAmmoCheat>());
}

void ModMenuModule::WeaponsMenu::GetAllWeapons()
{
	Game::Ped* playerPed = Game::Memory::GetPlayerPed();
	if (!playerPed) {
		spdlog::error("Could not give all weapons: player ped is null.");
		return;
	}

	for(int i = 0; i < 15; i++) {
		Game::Functions::AddWeapon(
			playerPed,
			0,
			static_cast<Game::WEAPON_INDEX>(i),
			99
		);
	}

	spdlog::info("All weapons given.");
}
