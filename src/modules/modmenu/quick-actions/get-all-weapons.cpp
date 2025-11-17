#include "get-all-weapons.h"
#include "../cheats/freeze-cop-value.h"
#include "../quick-action-registry.h"

static const std::string typeId = "ModMenu_GetAllWeapons";
static const std::wstring typeLabel = L"Get All Weapons";

ModMenuModule::GetAllWeaponsAction::GetAllWeaponsAction()
{
}

ModMenuModule::GetAllWeaponsAction::~GetAllWeaponsAction()
{
}

const std::string& ModMenuModule::GetAllWeaponsAction::GetTypeId()
{
	return typeId;
}

const std::wstring& ModMenuModule::GetAllWeaponsAction::GetTypeLabel()
{
	return typeLabel;
}

void ModMenuModule::GetAllWeaponsAction::Execute()
{
	Game::Ped* playerPed = Game::Memory::GetPlayerPed();
	if (!playerPed) {
		spdlog::warn("Could not give all weapons: player ped is null.");
		return;
	}

	for (int i = 0; i < 15; i++) {
		Game::Functions::AddWeapon(
			playerPed,
			0,
			static_cast<Game::WEAPON_INDEX>(i),
			99
		);
	}
}

const std::wstring& ModMenuModule::GetAllWeaponsAction::GetLabel() const
{
	return typeLabel;
}

REGISTER_QUICK_ACTION(GetAllWeaponsAction)
