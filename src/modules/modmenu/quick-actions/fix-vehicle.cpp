#include "fix-vehicle.h"
#include "../cheats/last-car.h"
#include "../utils/fix-car-utils.h"
#include "../toast-manager.h"
#include "../quick-action-registry.h"

static const std::string typeId = "ModMenu_FixVehicle";
static const std::wstring typeLabel = L"Fix last vehicle";

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

ModMenuModule::FixVehicleSegment* ModMenuModule::FixVehicleAction::CreateSegmentInstance()
{
	return new FixVehicleSegment();
}

bool ModMenuModule::FixVehicleAction::ValidateSegmentData(const FixVehicleSegmentData& data)
{
	if (!data.doExtinguish && !data.doFixEngineDamage && !data.doFixVisualDamage) {
		spdlog::warn("At least one fix option must be selected.");
		ToastManager::GetInstance()->Show({ L"At least one fix option must be selected", ToastType::Warning });
		return false;
	}

	return true;
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
		ToastManager::GetInstance()->Show({ L"No last vehicle to fix", ToastType::Warning });
		return;
	}

	const auto& data = m_data.has_value() ? m_data.value() : FixVehicleSegmentData{ true, true, true }; // backward compatibility

	if (data.doExtinguish) Utils::FixCar::ExtinguishCar(lastCar);
	if (data.doFixEngineDamage) Utils::FixCar::FixCarEngineDamage(lastCar);
	if (data.doFixVisualDamage) Utils::FixCar::FixCarVisualDamage(lastCar);

	ToastManager::GetInstance()->Show({ L"Fixed the vehicle" });
}

const std::wstring& ModMenuModule::FixVehicleAction::GetLabel() const
{
	return typeLabel;
}

REGISTER_QUICK_ACTION(FixVehicleAction)
