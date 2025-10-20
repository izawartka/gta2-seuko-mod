#include "infinite-ammo.h"
#include "../cheat-registry.h"

ModMenuModule::InfiniteAmmoCheat::InfiniteAmmoCheat() : ModMenuModule::CheatBase("Cheat_InfiniteAmmo_IsEnabled") {
}

ModMenuModule::InfiniteAmmoCheat::~InfiniteAmmoCheat()
{
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
