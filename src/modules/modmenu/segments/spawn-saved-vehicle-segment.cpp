#include "spawn-saved-vehicle-segment.h"
#include "../cheats/saved-cars.h"
#include "../root.h"

ModMenuModule::SpawnSavedVehicleSegment::SpawnSavedVehicleSegment(const std::string& persistencePrefix)
{
	m_persistencePrefix = persistencePrefix;
}

ModMenuModule::SpawnSavedVehicleSegment::~SpawnSavedVehicleSegment()
{

}

bool ModMenuModule::SpawnSavedVehicleSegment::ValidateSegment() const
{
	bool hasValue = m_savedCarNameController && m_savedCarNameController->GetValue().has_value();
	if (!hasValue) {
		spdlog::warn("No saved vehicles found");
		ToastManager::GetInstance()->Show({ L"No saved vehicles found", ToastType::Warning });
		return false;
	}

	std::wstring savedCarName = m_savedCarNameController->GetValue().value();
	SavedCarsCheat* savedCarsCheat = SavedCarsCheat::GetInstance();
	const auto& savedCarNames = savedCarsCheat->GetSavedCarsList();

	if (std::find(savedCarNames.begin(), savedCarNames.end(), savedCarName) == savedCarNames.end()) {
		spdlog::warn("Selected saved vehicle not found");
		ToastManager::GetInstance()->Show({ L"Selected saved vehicle not found", ToastType::Warning });
		return false;
	}

	return true;
}

std::optional<ModMenuModule::SpawnSavedVehicleSegmentData> ModMenuModule::SpawnSavedVehicleSegment::GetSegmentData() const
{
	if (!m_savedCarNameController) {
		spdlog::error("Cannot get segment data: savedCarNameController is not initialized.");
		return std::nullopt;
	}

	const auto& savedCarNameOpt = m_savedCarNameController->GetValue();
	if (!savedCarNameOpt.has_value()) {
		spdlog::error("Cannot get segment data: savedCarName has no value");
		return std::nullopt;
	}

	return SpawnSavedVehicleSegmentData{
		savedCarNameOpt.value()
	};
}

bool ModMenuModule::SpawnSavedVehicleSegment::SetSegmentData(const SpawnSavedVehicleSegmentData& data)
{
	if (!m_savedCarNameController) {
		spdlog::error("Cannot set segment data: savedCarNameController is not initialized.");
		return false;
	}
	m_savedCarNameController->SetValue(data.savedCarName);
	return true;
}

bool ModMenuModule::SpawnSavedVehicleSegment::Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent)
{
	CreateSegment(menu, parent);

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	SavedCarsCheat* savedCarsCheat = SavedCarsCheat::GetInstance();
	const auto& savedCarNames = savedCarsCheat->GetSavedCarsList();

	if (savedCarNames.empty()) {
		UiModule::Text* noSavedCarsText = m_menuController->CreateItem<UiModule::Text>(m_vertCont, L"No saved vehicles found", options.textSize);
		return true;
	}

	std::wstring selectedSavedCarName = savedCarNames[0];
	if (m_persistencePrefix.size()) {
		PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();
		auto loadedNameOpt = persistence->LoadOptional<std::wstring>(m_persistencePrefix + "_SelectedSavedCarName");
		if (loadedNameOpt.has_value() && std::find(savedCarNames.begin(), savedCarNames.end(), loadedNameOpt.value()) != savedCarNames.end()) {
			selectedSavedCarName = loadedNameOpt.value();
		}
	}

	UiModule::Text* savedCarNameText = m_menuController->CreateItem<UiModule::Text>(m_vertCont, L"", options.textSize);
	m_savedCarNameController = m_menuController->CreateLatestItemController<UiModule::SelectController<std::wstring>>(
		savedCarNameText,
		savedCarNames,
		selectedSavedCarName,
		UiModule::SelectControllerOptions{ L"Saved vehicle: #", L"#" }
	);

	return true;
}

void ModMenuModule::SpawnSavedVehicleSegment::Detach()
{
	if (m_persistencePrefix.size()) {
		auto selectedSavedCarNameOpt = m_savedCarNameController->GetValue();
		if (selectedSavedCarNameOpt.has_value()) {
			PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();
			persistence->Save(m_persistencePrefix + "_SelectedSavedCarName", selectedSavedCarNameOpt.value());
		}
	}

	m_savedCarNameController = nullptr;

	DestroySegment();
}
