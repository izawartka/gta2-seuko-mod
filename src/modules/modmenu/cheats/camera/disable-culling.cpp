#include "disable-culling.h"
#include "../../cheat-registry.h"

ModMenuModule::DisableCullingCheat::DisableCullingCheat() : ModMenuModule::CheatBase("Cheat_DisableCulling_IsEnabled") {
}

ModMenuModule::DisableCullingCheat::~DisableCullingCheat()
{
}

void ModMenuModule::DisableCullingCheat::OnFirstEnable()
{
}

void ModMenuModule::DisableCullingCheat::OnEnable()
{
	AddEventListener<CullingCheckEvent>(&ModMenuModule::DisableCullingCheat::OnCullingCheck);
}

void ModMenuModule::DisableCullingCheat::OnDisable()
{
	RemoveEventListener<CullingCheckEvent>();
}

void ModMenuModule::DisableCullingCheat::OnCullingCheck(CullingCheckEvent& event)
{
	event.SetModifiedDoCull(false);
}

REGISTER_CHEAT(DisableCullingCheat);
