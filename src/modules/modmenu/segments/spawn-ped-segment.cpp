#include "spawn-ped-segment.h"
#include "../root.h"
#include "../../../converters/yes-no.h"
#include "../menus/ped-template/options-menu.h"
#include "ped-preview-segment.h"

ModMenuModule::SpawnPedSegment::SpawnPedSegment()
{
	m_pedPreviewSegment = SegmentSupport::CreateSegment<PedPreviewSegment>();
}

ModMenuModule::SpawnPedSegment::SpawnPedSegment(std::string_view persistencePrefix)
{
	m_persistencePrefix = persistencePrefix;
	m_pedPreviewSegment = SegmentSupport::CreateSegment<PedPreviewSegment>();
}

ModMenuModule::SpawnPedSegment::~SpawnPedSegment()
{
	SegmentSupport::DeleteSegment(m_pedPreviewSegment);
}

std::optional<ModMenuModule::SpawnPedSegmentData> ModMenuModule::SpawnPedSegment::GetSegmentData() const
{
	if (!m_templateNameController || !m_inNearestCarController) {
		spdlog::warn("SpawnPedSegment: Cannot get segment data, controllers not initialized");
		return std::nullopt;
	}

	if (!m_tempTemplatePtr) {
		spdlog::warn("SpawnPedSegment: Cannot get segment data, template pointer not initialized");
		return std::nullopt;
	}

	return SpawnPedSegmentData{
		*m_tempTemplatePtr,
		m_inNearestCarController->GetValue().value_or(false)
	};
}

bool ModMenuModule::SpawnPedSegment::SetSegmentData(const SpawnPedSegmentData& data)
{
	if (!m_templateNameController || !m_inNearestCarController) {
		spdlog::warn("SpawnPedSegment: Cannot set segment data, controllers not initialized");
		return false;
	}

	if (!m_tempTemplatePtr) {
		spdlog::warn("SpawnPedSegment: Cannot set segment data, template pointer not initialized");
		return false;
	}

	*m_tempTemplatePtr = data.pedTemplate;
	m_templateNameController->SetValue(data.pedTemplate.isSaved ? data.pedTemplate.name : L"Custom");
	m_inNearestCarController->SetValue(data.inNearestCar);

	UpdatePreview();

	return true;
}

bool ModMenuModule::SpawnPedSegment::OnPassedMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	if (id == m_customizeBtnMenuItemId) {
		MenuManager::GetInstance()->AddMenu<PedTemplateOptionsMenu>(m_tempTemplateId);
		return true;
	}

	return false;
}

bool ModMenuModule::SpawnPedSegment::Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent)
{
	SegmentBase::CreateSegment(menu, parent);

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();

	// template
	PedTemplatesCheat* pedTemplatesCheat = PedTemplatesCheat::GetInstance();
	const auto& templateNameOptions = pedTemplatesCheat->GetTemplatesList();
	if (!templateNameOptions.size()) {
		spdlog::warn("SpawnPedSegment: No ped templates available");
		return false;
	}

	if (!m_tempTemplatePtr) {
		InitialCreateData();
	}

	const auto& selectedTemplateName = (m_tempTemplatePtr->isSaved) ? m_tempTemplatePtr->name : L"Custom";

	auto* templateNameText = m_menuController->CreateItem<UiModule::Text>(m_vertCont, L"", options.textSize);
	m_templateNameController = m_menuController->CreateLatestItemController<UiModule::SelectController<std::wstring>>(
		templateNameText,
		templateNameOptions,
		selectedTemplateName,
		UiModule::SelectControllerOptions{ L"Template: #", L"#" }
	);
	m_templateNameController->SetSaveCallback(std::bind(&SpawnPedSegment::OnTemplateControllerSave, this, std::placeholders::_1));

	// edit template
	m_menuController->CreateItem<UiModule::Text>(m_vertCont, L"Edit template", options.textSize);
	m_customizeBtnMenuItemId = m_menuController->GetLatestMenuItemId();

	// in nearest car
	bool selectedInNearestCar = m_persistencePrefix.empty() ? false : persistence->Load<bool>(m_persistencePrefix + "_InNearestCar", false);
	auto* inNearestCarText = m_menuController->CreateItem<UiModule::Text>(m_vertCont, L"", options.textSize);
	m_inNearestCarController = m_menuController->CreateLatestItemController<UiModule::SelectController<bool>>(
		inNearestCarText,
		std::vector<bool>{ false, true },
		selectedInNearestCar,
		UiModule::SelectControllerOptions{ L"In nearest vehicle: #", L"#" }
	);
	m_inNearestCarController->SetConverter<YesNoConverter>();

	// sprite preview
	uiRoot->AddComponent<UiModule::Spacer>(m_vertCont, 0, options.menuSpacerHeight);
	m_pedPreviewSegment->SetPedData(m_tempTemplatePtr->data);
	SegmentSupport::AttachSegment(m_pedPreviewSegment, menu, m_vertCont);

	return true;
}

void ModMenuModule::SpawnPedSegment::Detach()
{
	if (!m_persistencePrefix.empty() && m_inNearestCarController) {
		PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();
		persistence->Save<bool>(m_persistencePrefix + "_InNearestCar", m_inNearestCarController->GetValue().value_or(false));
	}

	m_templateNameController = nullptr;
	m_inNearestCarController = nullptr;
	SegmentSupport::DetachSegment(m_pedPreviewSegment);
	SegmentBase::DestroySegment();
}

void ModMenuModule::SpawnPedSegment::InitialCreateData()
{
	PedTemplatesCheat* pedTemplatesCheat = PedTemplatesCheat::GetInstance();
	const auto& templateOptionList = pedTemplatesCheat->GetTemplatesList();

	if (m_tempTemplateId != 0 && m_tempTemplateId != -1) {
		pedTemplatesCheat->DeleteTempTemplate(m_tempTemplateId);
	}

	std::string persistenceKey = m_persistencePrefix.empty() ? "" : (m_persistencePrefix + "_PedTemplate");
	m_tempTemplateId = pedTemplatesCheat->FindOrCreateTempTemplate(persistenceKey, templateOptionList[0]);
	m_tempTemplatePtr = pedTemplatesCheat->GetTempTemplate(m_tempTemplateId);
}

void ModMenuModule::SpawnPedSegment::OnTemplateControllerSave(const std::wstring& templateName)
{
	PedTemplatesCheat* pedTemplatesCheat = PedTemplatesCheat::GetInstance();
	const PedTemplate* pedTemplate = pedTemplatesCheat->GetTemplate(templateName);

	if(!pedTemplate) {
		InitialCreateData();
		UpdatePreview();
		return;
	}

	*m_tempTemplatePtr = *pedTemplate;
	UpdatePreview();
}

void ModMenuModule::SpawnPedSegment::UpdatePreview()
{
	m_pedPreviewSegment->SetPedData(m_tempTemplatePtr->data);
}
