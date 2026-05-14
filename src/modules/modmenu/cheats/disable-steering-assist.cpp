#include "disable-steering-assist.h"
#include "../cheat-registry.h"

ModMenuModule::DisableSteeringAssistCheat* ModMenuModule::DisableSteeringAssistCheat::m_instance = nullptr;

ModMenuModule::DisableSteeringAssistCheat::DisableSteeringAssistCheat() : ModMenuModule::CheatBase("Cheat_DisableSteeringAssist_IsEnabled") {
	assert(m_instance == nullptr && "DisableSteeringAssistCheat instance already exists");
	m_instance = this;
}

ModMenuModule::DisableSteeringAssistCheat::~DisableSteeringAssistCheat()
{
	m_instance = nullptr;
}

ModMenuModule::DisableSteeringAssistCheat* ModMenuModule::DisableSteeringAssistCheat::GetInstance()
{
	assert(m_instance && "DisableSteeringAssistCheat not initialized!");
	return m_instance;
}

void ModMenuModule::DisableSteeringAssistCheat::OnFirstEnable()
{

}

void ModMenuModule::DisableSteeringAssistCheat::OnEnable()
{
	AddEventListener<SteeringAssistEvent>(&ModMenuModule::DisableSteeringAssistCheat::OnSteeringAssist);
}

void ModMenuModule::DisableSteeringAssistCheat::OnDisable()
{
	RemoveEventListener<SteeringAssistEvent>();
}

void ModMenuModule::DisableSteeringAssistCheat::OnSteeringAssist(SteeringAssistEvent& event)
{
	event.SetDoAssist(false);
}

REGISTER_CHEAT(DisableSteeringAssistCheat)
