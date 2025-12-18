#include "disable-culling.h"
#include "../../cheat-registry.h"

ModMenuModule::DisableCullingCheat* ModMenuModule::DisableCullingCheat::m_instance = nullptr;

ModMenuModule::DisableCullingCheat::DisableCullingCheat() : ModMenuModule::CheatBase("Cheat_DisableCulling_IsEnabled") {
	assert(m_instance == nullptr && "DisableCullingCheat instance already exists");
	m_instance = this;
}

ModMenuModule::DisableCullingCheat::~DisableCullingCheat()
{
	m_instance = nullptr;
}

ModMenuModule::DisableCullingCheat* ModMenuModule::DisableCullingCheat::GetInstance()
{
	assert(m_instance && "DisableCullingCheat not initialized!");
	return m_instance;
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
