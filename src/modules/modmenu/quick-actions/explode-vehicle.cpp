#include "explode-vehicle.h"
#include "../utils/explode-last-car.h"
#include "../quick-action-registry.h"

static const std::string typeId = "ModMenu_ExplodeVehicle";
static const std::wstring typeLabel = L"Explode last vehicle";

ModMenuModule::ExplodeVehicleAction::ExplodeVehicleAction()
{}

ModMenuModule::ExplodeVehicleAction::~ExplodeVehicleAction()
{}

const std::string& ModMenuModule::ExplodeVehicleAction::GetTypeId()
{
	return typeId;
}

const std::wstring& ModMenuModule::ExplodeVehicleAction::GetTypeLabel()
{
	return typeLabel;
}

ModMenuModule::ExplodeVehicleSegment* ModMenuModule::ExplodeVehicleAction::CreateSegmentInstance()
{
	return new ExplodeVehicleSegment();
}

void ModMenuModule::ExplodeVehicleAction::Execute()
{
	if (!m_data.has_value()) {
		spdlog::error("ExplodeVehicleAction::Execute: No data to execute action.");
		return;
	}

	Utils::ExplodeLastCarWithToast(m_data.value().explosionSize, true);
}

const std::wstring& ModMenuModule::ExplodeVehicleAction::GetLabel() const
{
	return typeLabel;
}

REGISTER_QUICK_ACTION(ExplodeVehicleAction)
