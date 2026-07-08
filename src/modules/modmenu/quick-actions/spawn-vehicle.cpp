#include "spawn-vehicle.h"
#include "../utils/spawn-car-at-player.h"
#include "../../../converters/car-model-name.h"
#include "../toast-manager.h"
#include "../quick-action-registry.h"

static const std::string typeId = "ModMenu_SpawnVehicle";
static const std::wstring typeLabel = L"Spawn vehicle";

ModMenuModule::SpawnVehicleAction::SpawnVehicleAction()
{

}

ModMenuModule::SpawnVehicleAction::~SpawnVehicleAction()
{
}

const std::string& ModMenuModule::SpawnVehicleAction::GetTypeId()
{
	return typeId;
}

const std::wstring& ModMenuModule::SpawnVehicleAction::GetTypeLabel()
{
	return typeLabel;
}

ModMenuModule::SpawnVehicleSegment* ModMenuModule::SpawnVehicleAction::CreateSegmentInstance()
{
	return new SpawnVehicleSegment();
}

void ModMenuModule::SpawnVehicleAction::Execute()
{
	if (!m_data.has_value()) {
		spdlog::error("SpawnVehicleAction::Execute: No data to execute action.");
		return;
	}

	SpawnVehicleSegmentData data = m_data.value();
	std::wstring modelStr = CarModelNameConverter::ConvertToString(data.model);

	if (ModMenuModule::Utils::SpawnCarAtPlayer(data.model, data.remap, data.palette)) {
		ModMenuModule::ToastManager::GetInstance()->Show({ L"Spawned " + modelStr});
	}
	else {
		ModMenuModule::ToastManager::GetInstance()->Show({ L"Failed to spawn " + modelStr, ToastType::Error });
	}
}

const std::wstring& ModMenuModule::SpawnVehicleAction::GetLabel() const
{
	// label is exceptionally not cached OnDataChange because CarModelNameConverter does not work before game init
	if (m_data.has_value()) {
		static std::wstring label = L"Spawn " + CarModelNameConverter::ConvertToString(m_data->model);
		return label;
	}
	else {
		return typeLabel;
	}
}

REGISTER_QUICK_ACTION(SpawnVehicleAction)
