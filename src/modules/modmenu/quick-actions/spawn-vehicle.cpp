#include "spawn-vehicle.h"
#include "../utils/spawn-car-at-player.h"
#include "../../../converters/car-model.h"
#include "../toast-manager.h"
#include "../quick-action-registry.h"

static const std::string typeId = "ModMenu_SpawnVehicle";
static const std::wstring typeLabel = L"Spawn vehicle";

ModMenuModule::SpawnVehicleAction::SpawnVehicleAction()
{
	m_label = typeLabel;
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
	ModMenuModule::Utils::SpawnCarAtPlayer(data.model, data.remap, data.palette);
	ModMenuModule::ToastManager::GetInstance()->Show({ CarModelConverter::ConvertToString(data.model) + L" spawned" });
}

const std::wstring& ModMenuModule::SpawnVehicleAction::GetLabel() const
{
	return m_label;
}

void ModMenuModule::SpawnVehicleAction::OnDataChange()
{
	if (m_data.has_value()) {
		m_label = L"Spawn " + CarModelConverter::ConvertToString(m_data->model);
	}
	else {
		m_label = typeLabel;
	}
}

REGISTER_QUICK_ACTION_WITH_SEGMENT(SpawnVehicleAction)
