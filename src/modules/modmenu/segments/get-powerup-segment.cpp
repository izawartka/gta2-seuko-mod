#include "get-powerup-segment.h"
#include "../../../converters/powerup.h"
#include "../root.h"


ModMenuModule::GetPowerupSegment::GetPowerupSegment(const std::string& persistencePrefix)
{
	m_persistencePrefix = persistencePrefix;
}

ModMenuModule::GetPowerupSegment::~GetPowerupSegment()
{

}

std::optional<ModMenuModule::GetPowerupSegmentData> ModMenuModule::GetPowerupSegment::GetSegmentData() const
{
	if (!m_powerupTypeController) {
		spdlog::error("Cannot get segment data: controller is not initialized.");
		return std::nullopt;
	}

	return GetPowerupSegmentData{
		m_powerupTypeController->GetValue().value()
	};
}

bool ModMenuModule::GetPowerupSegment::SetSegmentData(const GetPowerupSegmentData& data)
{
	if (!m_powerupTypeController) {
		spdlog::error("Cannot set segment data: controller is not initialized.");
		return false;
	}

	m_powerupTypeController->SetValue(data.powerupType);
	return true;
}

bool ModMenuModule::GetPowerupSegment::Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent)
{
	CreateSegment(menu, parent);

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();

	const auto& powerupTypeOptionList = Game::Utils::GetAvailablePowerupTypes();
	auto selectedPowerupType = powerupTypeOptionList[0];

	if (m_persistencePrefix.size()) {
		selectedPowerupType = persistence->Load(m_persistencePrefix + "_SelectedPowerupType", powerupTypeOptionList[0]);
	}

	// power-up type
	UiModule::Text* powerupTypeText = m_menuController->CreateItem<UiModule::Text>(m_vertCont, L"", options.textSize);
	m_powerupTypeController = m_menuController->CreateLatestItemController<UiModule::SelectController<Game::POWERUP_TYPE>>(
		powerupTypeText,
		powerupTypeOptionList,
		selectedPowerupType,
		UiModule::SelectControllerOptions{ L"Power-up: #", L"#" }
	);
	m_powerupTypeController->SetConverter<PowerupConverter>();

	return true;
}

void ModMenuModule::GetPowerupSegment::Detach()
{
	if (m_persistencePrefix.size()) {
		PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();
		persistence->Save(m_persistencePrefix + "_SelectedPowerupType", m_powerupTypeController->GetValue().value());
	}

	DestroySegment();
}
