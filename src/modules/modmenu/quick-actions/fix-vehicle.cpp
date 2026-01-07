#include "fix-vehicle.h"
#include "../cheats/last-car.h"
#include "../toast-manager.h"
#include "../quick-action-registry.h"

static const std::string typeId = "ModMenu_FixVehicle";
static const std::wstring typeLabel = L"Fix last vehicle damage";

ModMenuModule::FixVehicleAction::FixVehicleAction()
{
}

ModMenuModule::FixVehicleAction::~FixVehicleAction()
{
}

const std::string& ModMenuModule::FixVehicleAction::GetTypeId()
{
	return typeId;
}

const std::wstring& ModMenuModule::FixVehicleAction::GetTypeLabel()
{
	return typeLabel;
}

void ModMenuModule::FixVehicleAction::Execute()
{
	LastCarCheat* lastCarCheat = LastCarCheat::GetInstance();
	if (!lastCarCheat->IsEnabled()) {
		spdlog::error("FixVehicleAction::Execute: LastCarCheat is not enabled.");
		return;
	}

	Game::Car* lastCar = lastCarCheat->GetLastCar();
	if (!lastCar) {
		spdlog::warn("FixVehicleAction::Execute: No last car to fix.");
		ModMenuModule::ToastManager::GetInstance()->Show({ L"No last vehicle to fix", ToastType::Warning });
		return;
	}

	lastCar->carDamage = 0;
	lastCar->fireState = 0;
	Game::Functions::ExtinguishCar(lastCar);
	Game::Functions::FixCarBrokenEngine(lastCar);

	spdlog::info("FixVehicleAction::Execute: Fixed damage for last car.");
	ModMenuModule::ToastManager::GetInstance()->Show({ L"Vehicle damage fixed" });
}

const std::wstring& ModMenuModule::FixVehicleAction::GetLabel() const
{
	return typeLabel;
}

REGISTER_QUICK_ACTION(FixVehicleAction)
