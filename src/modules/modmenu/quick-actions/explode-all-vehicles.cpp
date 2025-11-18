#include "explode-all-vehicles.h"
#include "../utils/explode-all-cars.h"
#include "../quick-action-registry.h"

static const std::string typeId = "ModMenu_ExplodeAllVeguckes";
static const std::wstring typeLabel = L"Explode all vehicles";

ModMenuModule::ExplodeAllVehiclesAction::ExplodeAllVehiclesAction()
{
}

ModMenuModule::ExplodeAllVehiclesAction::~ExplodeAllVehiclesAction()
{
}

const std::string& ModMenuModule::ExplodeAllVehiclesAction::GetTypeId()
{
	return typeId;
}

const std::wstring& ModMenuModule::ExplodeAllVehiclesAction::GetTypeLabel()
{
	return typeLabel;
}

void ModMenuModule::ExplodeAllVehiclesAction::Execute()
{
	ModMenuModule::Utils::ExplodeAllCars(true, Game::EXPLOSION_SIZE_MEDIUM);
}

const std::wstring& ModMenuModule::ExplodeAllVehiclesAction::GetLabel() const
{
	return typeLabel;
}

REGISTER_QUICK_ACTION(ExplodeAllVehiclesAction)
