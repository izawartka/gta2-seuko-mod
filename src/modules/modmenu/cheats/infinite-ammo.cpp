#include "infinite-ammo.h"
#include "../cheat-registry.h"

ModMenuModule::InfiniteAmmoCheat* ModMenuModule::InfiniteAmmoCheat::m_instance = nullptr;

ModMenuModule::InfiniteAmmoCheat::InfiniteAmmoCheat() : ModMenuModule::CheatBase("Cheat_InfiniteAmmo_IsEnabled") {
	assert(m_instance == nullptr && "InfiniteAmmoCheat instance already exists");
	m_instance = this;
}

ModMenuModule::InfiniteAmmoCheat::~InfiniteAmmoCheat()
{
	m_instance = nullptr;
}

ModMenuModule::InfiniteAmmoCheat* ModMenuModule::InfiniteAmmoCheat::GetInstance()
{
	assert(m_instance && "InfiniteAmmoCheat not initialized!");
	return m_instance;
}

void ModMenuModule::InfiniteAmmoCheat::OnFirstEnable()
{

}

void ModMenuModule::InfiniteAmmoCheat::OnEnable()
{
	AddEventListener<IsAmmoKfCallEvent>(&ModMenuModule::InfiniteAmmoCheat::OnIsAmmoKfCall);
}

void ModMenuModule::InfiniteAmmoCheat::OnDisable()
{
	RemoveEventListener<IsAmmoKfCallEvent>();
}

void ModMenuModule::InfiniteAmmoCheat::OnIsAmmoKfCall(IsAmmoKfCallEvent& event)
{
	event.SetModifiedValue(true);
}

REGISTER_CHEAT(InfiniteAmmoCheat)
