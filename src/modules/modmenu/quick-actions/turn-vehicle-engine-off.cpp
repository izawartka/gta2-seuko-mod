#include "turn-vehicle-engine-off.h"
#include "../utils/turn-last-car-engine-off.h"
#include "../quick-action-registry.h"

static const std::string typeId = "ModMenu_TurnVehicleEngineOff";
static const std::wstring typeLabel = L"Turn last vehicle engine off";

ModMenuModule::TurnVehicleEngineOffAction::TurnVehicleEngineOffAction()
{}

ModMenuModule::TurnVehicleEngineOffAction::~TurnVehicleEngineOffAction()
{}

const std::string& ModMenuModule::TurnVehicleEngineOffAction::GetTypeId()
{
	return typeId;
}

const std::wstring& ModMenuModule::TurnVehicleEngineOffAction::GetTypeLabel()
{
	return typeLabel;
}

void ModMenuModule::TurnVehicleEngineOffAction::Execute()
{
	Utils::TurnLastCarEngineOffWithToast(true);
}

const std::wstring& ModMenuModule::TurnVehicleEngineOffAction::GetLabel() const
{
	return typeLabel;
}

REGISTER_QUICK_ACTION(TurnVehicleEngineOffAction)
