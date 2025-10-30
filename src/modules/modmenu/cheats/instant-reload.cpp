#include "instant-reload.h"
#include "../cheat-registry.h"

ModMenuModule::InstantReloadCheat::InstantReloadCheat() : ModMenuModule::CheatBase("Cheat_InstantReload_IsEnabled") {
}

ModMenuModule::InstantReloadCheat::~InstantReloadCheat()
{
}

void ModMenuModule::InstantReloadCheat::OnFirstEnable()
{

}

void ModMenuModule::InstantReloadCheat::OnEnable()
{
	AddEventListener<TryShootWeaponEvent>(&ModMenuModule::InstantReloadCheat::OnTryShootWeapon);
}

void ModMenuModule::InstantReloadCheat::OnDisable()
{
	RemoveEventListener<TryShootWeaponEvent>();
}

void ModMenuModule::InstantReloadCheat::OnTryShootWeapon(TryShootWeaponEvent& event)
{
	Game::WEAPON_STRUCT* weapon = event.GetWeapon();
	if (!weapon) return;

	Game::Ped* ped = weapon->ped;
	Game::Ped* playerPed = Game::Memory::GetPlayerPed();
	if (ped != playerPed) return;

	weapon->timeToReload = 0;
}

REGISTER_CHEAT(InstantReloadCheat)
