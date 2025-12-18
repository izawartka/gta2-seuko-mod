#include "weapons-menu.h"
#include "get-weapon-menu.h"
#include "../cheats/infinite-ammo.h"
#include "../cheats/instant-reload.h"
#include "../../../converters/yes-no.h"
#include "../../../converters/weapon.h"
#include "../../../converters/ammo.h"
#include "../utils/get-all-weapons.h"
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
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

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
	m_infiniteAmmoController->SetSaveCallback([](bool newValue) {
		InfiniteAmmoCheat::GetInstance()->SetEnabled(newValue);
	});

	// instant reload
	UiModule::Text* instantReloadText = m_menuController->CreateItem<UiModule::Text>(vertCont, L"", options.textSize);
	m_instantReloadController = m_menuController->CreateLatestItemController<UiModule::SelectController<bool>>(
		instantReloadText,
		UiModule::SelectOptionList<bool>{ false, true },
		std::nullopt,
		UiModule::SelectControllerOptions{ L"Instant reload: #", L"#" }
	);
	m_instantReloadController->SetConverter<YesNoConverter>();
	m_instantReloadController->SetSaveCallback([](bool newValue) {
		InstantReloadCheat::GetInstance()->SetEnabled(newValue);
	});

	// current weapon
	auto selectedWeaponResolver = Core::MakeResolver(
		Game::Memory::GetPlayerPed,
		mem(&Game::Ped::selectedWeapon),
		mem(&Game::WEAPON_STRUCT::id)
	);

	UiModule::Spacer* weaponSpacer = uiRoot->AddComponent<UiModule::Spacer>(vertCont, 0, options.menuSpacerHeight);
	UiModule::Margin* weaponTextMargin = uiRoot->AddComponent<UiModule::Margin>(vertCont, options.menuControllerOptions.createdSelectableOptions.markerOffsetX, 0);
	UiModule::Text* weaponText = uiRoot->AddComponent<UiModule::Text>(weaponTextMargin, L"", options.textSize);
	m_weaponController = uiRoot->AddController<UiModule::VarTextController<Game::WEAPON_INDEX>>(
		weaponText,
		selectedWeaponResolver,
		UiModule::VarTextControllerOptions{ L"Weapon: #", L"#", L"None" }
	);
	m_weaponController->SetConverter<WeaponConverter>();

	// ammo
	auto ammoResolver = Core::MakeResolver(
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

	SetPreviousSelectedIndex();

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
		ModMenuModule::MenuManager::GetInstance()->RemoveLastMenu();
		break;
	case 1: // Get weapon
		ModMenuModule::MenuManager::GetInstance()->AddMenu<ModMenuModule::GetWeaponMenu>();
		break;
	case 2: // Get all weapons
		ModMenuModule::Utils::GetAllWeapons();
		break;
	default:
		break;
	}
}

void ModMenuModule::WeaponsMenu::OnCheatStateChange(CheatStateEvent& event)
{
	if (event.GetCheatType() == typeid(InfiniteAmmoCheat)) {
		m_infiniteAmmoController->SetValue(event.IsEnabled());
	} else if (event.GetCheatType() == typeid(InstantReloadCheat)) {
		m_instantReloadController->SetValue(event.IsEnabled());
	}
}

void ModMenuModule::WeaponsMenu::UpdateCheatStates()
{
	m_infiniteAmmoController->SetValue(InfiniteAmmoCheat::GetInstance()->IsEnabled());
	m_instantReloadController->SetValue(InstantReloadCheat::GetInstance()->IsEnabled());
}
