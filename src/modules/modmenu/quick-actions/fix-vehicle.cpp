#include "fix-vehicle.h"
#include "../cheats/last-car.h"
#include "../utils/fix-car-utils.h"
#include "../toast-manager.h"
#include "../quick-action-registry.h"

static const std::string typeId = "ModMenu_FixVehicle";
static const std::wstring typeLabel = L"Fix last vehicle engine damage";

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

	Utils::FixCar::ExtinguishCar(lastCar);
	Utils::FixCar::FixCarEngineDamage(lastCar);

	ModMenuModule::ToastManager::GetInstance()->Show({ L"Fixed vehicle engine damage" });
}

const std::wstring& ModMenuModule::FixVehicleAction::GetLabel() const
{
	return typeLabel;
}

REGISTER_QUICK_ACTION(FixVehicleAction)
