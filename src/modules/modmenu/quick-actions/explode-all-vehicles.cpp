#include "explode-all-vehicles.h"
#include "../utils/explode-all-cars.h"
#include "../toast-manager.h"
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

ModMenuModule::ExplodeAllVehiclesSegment* ModMenuModule::ExplodeAllVehiclesAction::CreateSegmentInstance()
{
	return new ModMenuModule::ExplodeAllVehiclesSegment();
}

void ModMenuModule::ExplodeAllVehiclesAction::Execute()
{
	if (!m_data.has_value()) {
		spdlog::error("ExplodeAllVehiclesAction::Execute: No data to execute action.");
		return;
	}

	ExplodeAllVehiclesSegmentData data = m_data.value();
	ModMenuModule::Utils::ExplodeAllCars(data.excludePlayerVehicle, data.explosionSize);

	spdlog::info("ExplodeAllVehiclesAction::Execute: All vehicles exploded.");
	ModMenuModule::ToastManager::GetInstance()->Show({ L"All vehicles exploded" });
}

const std::wstring& ModMenuModule::ExplodeAllVehiclesAction::GetLabel() const
{
	return typeLabel;
}

REGISTER_QUICK_ACTION(ExplodeAllVehiclesAction)
