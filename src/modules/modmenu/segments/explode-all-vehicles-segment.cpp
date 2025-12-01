#include "explode-all-vehicles-segment.h"
#include "../../../converters/yes-no.h"
#include "../../../converters/explosion-size.h"
#include "../root.h"


ModMenuModule::ExplodeAllVehiclesSegment::ExplodeAllVehiclesSegment(const std::string& persistencePrefix)
{
	m_persistencePrefix = persistencePrefix;
}

ModMenuModule::ExplodeAllVehiclesSegment::~ExplodeAllVehiclesSegment()
{

}

bool ModMenuModule::ExplodeAllVehiclesSegment::Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent)
{
	CreateSegment(menu, parent);

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();

	auto selectedExcludePlayerVehicle = true;
	const auto& explosionSizeOptionList = Game::Utils::GetAvailableExplosionSizes();
	auto selectedExplosionSize = Game::EXPLOSION_SIZE_MEDIUM;

	if (m_persistencePrefix.size()) {
		selectedExcludePlayerVehicle = persistence->Load(m_persistencePrefix + "_SelectedExcludePlayerVehicle", true);
		selectedExplosionSize = persistence->Load(m_persistencePrefix + "_SelectedExplosionSize", Game::EXPLOSION_SIZE_MEDIUM);
	}

	// exclude player vehicle
	UiModule::Text* excludePlayerVehicleText = m_menuController->CreateItem<UiModule::Text>(m_vertCont, L"", options.textSize);
	m_excludePlayerVehicleController = m_menuController->CreateLatestItemController<UiModule::SelectController<bool>>(
		excludePlayerVehicleText,
		std::vector<bool>{ false, true },
		selectedExcludePlayerVehicle,
		UiModule::SelectControllerOptions{ L"Exclude Player Vehicle: #", L"#" }
	);
	m_excludePlayerVehicleController->SetConverter<YesNoConverter>();

	// explosion size
	UiModule::Text* explosionSizeText = m_menuController->CreateItem<UiModule::Text>(m_vertCont, L"", options.textSize);
	m_explosionSizeController = m_menuController->CreateLatestItemController<UiModule::SelectController<Game::EXPLOSION_SIZE>>(
		explosionSizeText,
		explosionSizeOptionList,
		selectedExplosionSize,
		UiModule::SelectControllerOptions{ L"Explosion Size: #", L"#" }
	);
	m_explosionSizeController->SetConverter<ExplosionSizeConverter>();

	return true;
}

void ModMenuModule::ExplodeAllVehiclesSegment::Detach()
{
	if (m_persistencePrefix.size()) {
		PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();
		persistence->Save(m_persistencePrefix + "_SelectedExcludePlayerVehicle", m_excludePlayerVehicleController->GetValue().value());
		persistence->Save(m_persistencePrefix + "_SelectedExplosionSize", m_explosionSizeController->GetValue().value());
	}

	DestroySegment();
}

std::optional<ModMenuModule::ExplodeAllVehiclesSegmentData> ModMenuModule::ExplodeAllVehiclesSegment::GetSegmentData() const
{
	if (!m_excludePlayerVehicleController || !m_explosionSizeController) {
		spdlog::error("Cannot get segment data: controllers are not initialized.");
		return std::nullopt;
	}

	return ExplodeAllVehiclesSegmentData{
		m_excludePlayerVehicleController->GetValue().value(),
		m_explosionSizeController->GetValue().value()
	};
}

bool ModMenuModule::ExplodeAllVehiclesSegment::SetSegmentData(const ExplodeAllVehiclesSegmentData& data)
{
	if (!m_excludePlayerVehicleController || !m_explosionSizeController) {
		spdlog::error("Cannot set segment data: controllers are not initialized.");
		return false;
	}

	m_excludePlayerVehicleController->SetValue(data.excludePlayerVehicle);
	m_explosionSizeController->SetValue(data.explosionSize);
	return true;
}
