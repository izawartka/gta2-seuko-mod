#include "clear-screen.h"
#include "../../cheat-registry.h"

ModMenuModule::ClearScreenCheat* ModMenuModule::ClearScreenCheat::m_instance = nullptr;

ModMenuModule::ClearScreenCheat::ClearScreenCheat() : ModMenuModule::CheatBase("Cheat_ClearScreen_IsEnabled") {
	assert(m_instance == nullptr && "ClearScreenCheat instance already exists");
	m_instance = this;
}

ModMenuModule::ClearScreenCheat::~ClearScreenCheat()
{
	m_instance = nullptr;
}

ModMenuModule::ClearScreenCheat* ModMenuModule::ClearScreenCheat::GetInstance()
{
	assert(m_instance && "ClearScreenCheat not initialized!");
	return m_instance;
}

void ModMenuModule::ClearScreenCheat::OnFirstEnable()
{
}

void ModMenuModule::ClearScreenCheat::OnEnable()
{
	AddEventListener<ClearScreenEvent>(&ModMenuModule::ClearScreenCheat::OnClearScreen);
}

void ModMenuModule::ClearScreenCheat::OnDisable()
{
	RemoveEventListener<ClearScreenEvent>();
}

void ModMenuModule::ClearScreenCheat::OnClearScreen(ClearScreenEvent& event)
{
	event.SetDoClear(true);
}

REGISTER_CHEAT(ClearScreenCheat);
