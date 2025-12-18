#include "instant-reload.h"
#include "../cheat-registry.h"

ModMenuModule::InstantReloadCheat* ModMenuModule::InstantReloadCheat::m_instance = nullptr;

ModMenuModule::InstantReloadCheat::InstantReloadCheat() : ModMenuModule::CheatBase("Cheat_InstantReload_IsEnabled") {
	assert(m_instance == nullptr && "InstantReloadCheat instance already exists");
	m_instance = this;
}

ModMenuModule::InstantReloadCheat::~InstantReloadCheat()
{
	m_instance = nullptr;
}

ModMenuModule::InstantReloadCheat* ModMenuModule::InstantReloadCheat::GetInstance()
{
	assert(m_instance && "InstantReloadCheat not initialized!");
	return m_instance;
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
