#include "spawn-vehicle.h"
#include "../utils/spawn-car-at-player.h"
#include "../../../converters/car-model.h"
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

ModMenuModule::Segment<ModMenuModule::SpawnVehicleSegmentData>* ModMenuModule::SpawnVehicleAction::CreateSegmentInstance()
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
}

const std::wstring& ModMenuModule::SpawnVehicleAction::GetLabel() const
{
	return m_label;
}

std::vector<uint8_t> ModMenuModule::SpawnVehicleAction::SerializeData() const
{
	if (!m_data.has_value()) {
		spdlog::error("SpawnVehicleAction::SerializeData: No data to serialize.");
		return {};
	}
	const SpawnVehicleSegmentData& data = m_data.value();
	std::vector<uint8_t> buffer;
	buffer.resize(sizeof(data));
	std::memcpy(buffer.data(), &data, sizeof(data));
	return buffer;
}

bool ModMenuModule::SpawnVehicleAction::DeserializeData(const std::vector<uint8_t>& data)
{
	if (data.size() != sizeof(SpawnVehicleSegmentData)) {
		spdlog::error("SpawnVehicleAction::DeserializeData: Invalid data size.");
		return false;
	}
	SpawnVehicleSegmentData deserializedData;
	std::memcpy(&deserializedData, data.data(), sizeof(SpawnVehicleSegmentData));
	m_data = deserializedData;
	OnDataChange();
	return true;
}

void ModMenuModule::SpawnVehicleAction::OnDataChange()
{
	m_label = L"Spawn " + CarModelConverter::ConvertToString(m_data->model);
}

REGISTER_QUICK_ACTION_WITH_SEGMENT(SpawnVehicleAction)
