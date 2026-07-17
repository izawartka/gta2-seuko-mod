#include "explode-vehicle-segment.h"
#include "../../../converters/explosion-size.h"
#include "../root.h"


ModMenuModule::ExplodeVehicleSegment::ExplodeVehicleSegment(std::string_view persistencePrefix)
{
	m_persistencePrefix = persistencePrefix;
}

ModMenuModule::ExplodeVehicleSegment::~ExplodeVehicleSegment()
{

}

std::optional<ModMenuModule::ExplodeVehicleSegmentData> ModMenuModule::ExplodeVehicleSegment::GetSegmentData() const
{
	if (!m_explosionSizeController) {
		spdlog::error("Cannot get segment data: controller is not initialized.");
		return std::nullopt;
	}

	return ExplodeVehicleSegmentData{
		m_explosionSizeController->GetValue().value()
	};
}

bool ModMenuModule::ExplodeVehicleSegment::SetSegmentData(const ExplodeVehicleSegmentData& data)
{
	if (!m_explosionSizeController) {
		spdlog::error("Cannot set segment data: controller is not initialized.");
		return false;
	}

	m_explosionSizeController->SetValue(data.explosionSize);
	return true;
}

bool ModMenuModule::ExplodeVehicleSegment::Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent)
{
	CreateSegment(menu, parent);

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();

	const auto& explosionSizeOptionList = Game::Utils::GetAvailableExplosionSizes();
	auto selectedExplosionSize = Game::EXPLOSION_SIZE_MEDIUM;

	if (m_persistencePrefix.size()) {
		selectedExplosionSize = persistence->Load(m_persistencePrefix + "_SelectedExplosionSize", Game::EXPLOSION_SIZE_MEDIUM);
	}

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

void ModMenuModule::ExplodeVehicleSegment::Detach()
{
	if (m_persistencePrefix.size()) {
		PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();
		persistence->Save(m_persistencePrefix + "_SelectedExplosionSize", m_explosionSizeController->GetValue().value());
	}

	DestroySegment();
}
