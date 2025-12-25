#include "spawn-saved-vehicle.h"
#include "../cheats/saved-cars.h"
#include "../toast-manager.h"
#include "../quick-action-registry.h"

static const std::string typeId = "ModMenu_SpawnSavedVehicle";
static const std::wstring typeLabel = L"Spawn saved vehicle";

ModMenuModule::SpawnSavedVehicleAction::SpawnSavedVehicleAction()
{
	m_label = typeLabel;
}

ModMenuModule::SpawnSavedVehicleAction::~SpawnSavedVehicleAction()
{
}

const std::string& ModMenuModule::SpawnSavedVehicleAction::GetTypeId()
{
	return typeId;
}

const std::wstring& ModMenuModule::SpawnSavedVehicleAction::GetTypeLabel()
{
	return typeLabel;
}

ModMenuModule::SpawnSavedVehicleSegment* ModMenuModule::SpawnSavedVehicleAction::CreateSegmentInstance()
{
	return new SpawnSavedVehicleSegment();
}

void ModMenuModule::SpawnSavedVehicleAction::Execute()
{
	if (!m_data.has_value()) {
		spdlog::error("SpawnSavedVehicleAction::Execute: No data to execute action.");
		return;
	}

	SavedCarsCheat* savedCarsCheat = SavedCarsCheat::GetInstance();
	if (!savedCarsCheat->IsEnabled()) {
		spdlog::error("SpawnSavedVehicleAction::Execute: SavedCarsCheat is not enabled.");
		return;
	}

	SpawnSavedVehicleSegmentData data = m_data.value();

	if (savedCarsCheat->SpawnCar(data.savedCarName)) {
		ModMenuModule::ToastManager::GetInstance()->Show({ L"Spawned " + data.savedCarName });
	}
	else {
		ModMenuModule::ToastManager::GetInstance()->Show({ L"Failed to spawn " + data.savedCarName, ToastType::Error });
	}
}

const std::wstring& ModMenuModule::SpawnSavedVehicleAction::GetLabel() const
{
	return m_label;
}

std::vector<uint8_t> ModMenuModule::SpawnSavedVehicleAction::SerializeData() const
{
	if (!m_data.has_value()) {
		spdlog::error("SpawnSavedVehicleAction::SerializeData: No data to serialize.");
		return {};
	}

	const SpawnSavedVehicleSegmentData& data = m_data.value();
	std::vector<uint8_t> buffer;
	size_t bufferSize = data.savedCarName.size() * sizeof(wchar_t);
	buffer.resize(bufferSize);
	std::memcpy(buffer.data(), data.savedCarName.data(), bufferSize);
	return buffer;
}

bool ModMenuModule::SpawnSavedVehicleAction::DeserializeData(const std::vector<uint8_t>& data)
{
	if (data.size() % sizeof(wchar_t) != 0) {
		spdlog::error("SpawnSavedVehicleAction::DeserializeData: Invalid data size.");
		return false;
	}
	size_t wcharCount = data.size() / sizeof(wchar_t);
	std::wstring savedCarName(wcharCount, L'\0');
	std::memcpy(&savedCarName[0], data.data(), data.size());
	m_data = SpawnSavedVehicleSegmentData{ savedCarName };
	OnDataChange();
	return true;
}

void ModMenuModule::SpawnSavedVehicleAction::OnDataChange()
{
	if (m_data.has_value()) {
		m_label = L"Spawn " + m_data->savedCarName;
	}
	else {
		m_label = typeLabel;
	}
}

REGISTER_QUICK_ACTION_WITH_SEGMENT(SpawnSavedVehicleAction)
