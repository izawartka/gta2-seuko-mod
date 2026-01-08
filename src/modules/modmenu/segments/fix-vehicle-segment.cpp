#include "fix-vehicle-segment.h"
#include "../../../converters/yes-no.h"
#include "../root.h"
#include "../toast-manager.h"

ModMenuModule::FixVehicleSegment::FixVehicleSegment(const std::string& persistencePrefix)
{
	m_persistencePrefix = persistencePrefix;
}

ModMenuModule::FixVehicleSegment::~FixVehicleSegment()
{

}

bool ModMenuModule::FixVehicleSegment::ValidateSegment() const
{
	const auto& segmentDataOpt = GetSegmentData();
	if (!segmentDataOpt.has_value()) return false;

	const auto& segmentData = segmentDataOpt.value();

	if (!segmentData.doExtinguish && !segmentData.doFixEngineDamage && !segmentData.doFixVisualDamage) {
		spdlog::warn("At least one fix option must be selected.");
		ToastManager::GetInstance()->Show({ L"At least one fix option must be selected", ToastType::Warning });
		return false;
	}

	return true;
}

std::optional<ModMenuModule::FixVehicleSegmentData> ModMenuModule::FixVehicleSegment::GetSegmentData() const
{
	if (!m_doExtinguishController || !m_doFixEngineDamageController || !m_doFixVisualDamageController) {
		spdlog::error("Cannot get segment data: controllers are not initialized.");
		return std::nullopt;
	}

	return FixVehicleSegmentData{
		m_doExtinguishController->GetValue().value(),
		m_doFixEngineDamageController->GetValue().value(),
		m_doFixVisualDamageController->GetValue().value()
	};
}

bool ModMenuModule::FixVehicleSegment::SetSegmentData(const FixVehicleSegmentData& data)
{
	if (!m_doExtinguishController || !m_doFixEngineDamageController || !m_doFixVisualDamageController) {
		spdlog::error("Cannot set segment data: controllers are not initialized.");
		return false;
	}

	m_doExtinguishController->SetValue(data.doExtinguish);
	m_doFixEngineDamageController->SetValue(data.doFixEngineDamage);
	m_doFixVisualDamageController->SetValue(data.doFixVisualDamage);

	return true;
}

bool ModMenuModule::FixVehicleSegment::Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent)
{
	CreateSegment(menu, parent);

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();

	bool selectedDoExtinguish = true;
	bool selectedDoFixEngineDamage = true;
	bool selectedDoFixVisualDamage = true;

	if (m_persistencePrefix.size()) {
		selectedDoExtinguish = persistence->Load(m_persistencePrefix + "_SelectedDoExtinguish", true);
		selectedDoFixEngineDamage = persistence->Load(m_persistencePrefix + "_SelectedDoFixEngineDamage", true);
		selectedDoFixVisualDamage = persistence->Load(m_persistencePrefix + "_SelectedDoFixVisualDamage", true);
	}

	// extinguish
	UiModule::Text* doExtinguishText = m_menuController->CreateItem<UiModule::Text>(m_vertCont, L"", options.textSize);
	m_doExtinguishController = m_menuController->CreateLatestItemController<UiModule::SelectController<bool>>(
		doExtinguishText,
		std::vector<bool>{ false, true },
		selectedDoExtinguish,
		UiModule::SelectControllerOptions{ L"Extinguish: #", L"#" }
	);
	m_doExtinguishController->SetConverter<YesNoConverter>();

	// fix engine damage
	UiModule::Text* doFixEngineDamageText = m_menuController->CreateItem<UiModule::Text>(m_vertCont, L"", options.textSize);
	m_doFixEngineDamageController = m_menuController->CreateLatestItemController<UiModule::SelectController<bool>>(
		doFixEngineDamageText,
		std::vector<bool>{ false, true },
		selectedDoFixEngineDamage,
		UiModule::SelectControllerOptions{ L"Fix Engine Damage: #", L"#" }
	);
	m_doFixEngineDamageController->SetConverter<YesNoConverter>();

	// fix visual damage
	UiModule::Text* doFixVisualDamageText = m_menuController->CreateItem<UiModule::Text>(m_vertCont, L"", options.textSize);
	m_doFixVisualDamageController = m_menuController->CreateLatestItemController<UiModule::SelectController<bool>>(
		doFixVisualDamageText,
		std::vector<bool>{ false, true },
		selectedDoFixVisualDamage,
		UiModule::SelectControllerOptions{ L"Fix Visual Damage: #", L"#" }
	);
	m_doFixVisualDamageController->SetConverter<YesNoConverter>();

	return true;
}

void ModMenuModule::FixVehicleSegment::Detach()
{
	if (m_persistencePrefix.size()) {
		PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();
		persistence->Save(m_persistencePrefix + "_SelectedDoExtinguish", m_doExtinguishController->GetValue().value());
		persistence->Save(m_persistencePrefix + "_SelectedDoFixEngineDamage", m_doFixEngineDamageController->GetValue().value());
		persistence->Save(m_persistencePrefix + "_SelectedDoFixVisualDamage", m_doFixVisualDamageController->GetValue().value());
	}

	DestroySegment();
}
