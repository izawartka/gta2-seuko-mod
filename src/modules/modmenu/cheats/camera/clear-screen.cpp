#include "clear-screen.h"
#include "../../cheat-registry.h"

ModMenuModule::ClearScreenCheat::ClearScreenCheat() : ModMenuModule::CheatBase("Cheat_ClearScreen_IsEnabled") {
}

ModMenuModule::ClearScreenCheat::~ClearScreenCheat()
{
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
