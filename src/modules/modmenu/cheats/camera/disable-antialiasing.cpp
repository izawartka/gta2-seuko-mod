#include "disable-antialiasing.h"
#include "../../cheat-registry.h"

ModMenuModule::DisableAntialiasingCheat* ModMenuModule::DisableAntialiasingCheat::m_instance = nullptr;

ModMenuModule::DisableAntialiasingCheat::DisableAntialiasingCheat() : ModMenuModule::CheatBase("Cheat_DisableAntialiasing_IsEnabled") {
	assert(m_instance == nullptr && "DisableAntialiasingCheat instance already exists");
	m_instance = this;
}

ModMenuModule::DisableAntialiasingCheat::~DisableAntialiasingCheat()
{
	m_instance = nullptr;
}

ModMenuModule::DisableAntialiasingCheat* ModMenuModule::DisableAntialiasingCheat::GetInstance()
{
	assert(m_instance && "DisableAntialiasingCheat not initialized!");
	return m_instance;
}

void ModMenuModule::DisableAntialiasingCheat::OnFirstEnable()
{
}

void ModMenuModule::DisableAntialiasingCheat::OnEnable()
{
	if (!*Game::Memory::GetIsRendererLoaded()) {
		AddEventListener<RendererLoadEvent>(&ModMenuModule::DisableAntialiasingCheat::OnRendererLoad, true);
	}
	else {
		AddEventListener<RendererAntialiasingCheckEvent>(&ModMenuModule::DisableAntialiasingCheat::OnAntialiasingCheck);
	}
}

void ModMenuModule::DisableAntialiasingCheat::OnDisable()
{
	if (HasEventListener<RendererLoadEvent>()) RemoveEventListener<RendererLoadEvent>();
	if (HasEventListener<RendererAntialiasingCheckEvent>()) RemoveEventListener<RendererAntialiasingCheckEvent>();
}

void ModMenuModule::DisableAntialiasingCheat::OnRendererLoad(RendererLoadEvent& event)
{
	if (IsEnabled()) AddEventListener<RendererAntialiasingCheckEvent>(&ModMenuModule::DisableAntialiasingCheat::OnAntialiasingCheck);
}

void ModMenuModule::DisableAntialiasingCheat::OnAntialiasingCheck(RendererAntialiasingCheckEvent& event)
{
	event.SetModifiedDoAntialiasing(false);
}

REGISTER_CHEAT(DisableAntialiasingCheat);
