#include "spawn-ped-action-segment.h"
#include "../cheats/ped-templates.h"
#include "../root.h"
#include "../../../converters/yes-no.h"
#include "ped-preview-segment.h"
#include "../toast-manager.h"

ModMenuModule::SpawnPedActionSegment::SpawnPedActionSegment()
{
	m_pedPreviewSegment = SegmentSupport::CreateSegment<PedPreviewSegment>();
}

ModMenuModule::SpawnPedActionSegment::~SpawnPedActionSegment()
{
	SegmentSupport::DeleteSegment(m_pedPreviewSegment);
}

bool ModMenuModule::SpawnPedActionSegment::ValidateSegment()
{
	if (!m_pedPreviewSegment || !m_templateNameController) return false;

	PedTemplatesCheat* pedTemplatesCheat = PedTemplatesCheat::GetInstance();
	const std::wstring& pedTemplateName = m_templateNameController->GetValue().value();
	const auto* pedTemplate = pedTemplatesCheat->GetTemplate(pedTemplateName);

	if (!pedTemplate) {
		spdlog::warn(L"SpawnPedActionSegment: Validation failed, ped template \"{}\" not found.", pedTemplateName);
		ToastManager::GetInstance()->Show({ L"Selected template not found", ToastType::Warning });
		return false;
	}

	return true;
}

std::optional<ModMenuModule::SpawnPedActionSegmentData> ModMenuModule::SpawnPedActionSegment::GetSegmentData() const
{
	if (!m_templateNameController || !m_inNearestCarController) {
		spdlog::warn("SpawnPedActionSegment: Cannot get segment data, controllers not initialized");
		return std::nullopt;
	}

	return SpawnPedActionSegmentData{
		m_templateNameController->GetValue().value_or(L""),
		m_inNearestCarController->GetValue().value_or(false)
	};
}

bool ModMenuModule::SpawnPedActionSegment::SetSegmentData(const SpawnPedActionSegmentData& data)
{
	if (!m_templateNameController || !m_inNearestCarController) {
		spdlog::warn("SpawnPedActionSegment: Cannot set segment data, controllers not initialized");
		return false;
	}

	m_templateNameController->SetValue(data.pedTemplateName);
	m_inNearestCarController->SetValue(data.inNearestCar);

	UpdatePreview();

	return true;
}

bool ModMenuModule::SpawnPedActionSegment::Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent)
{
	SegmentBase::CreateSegment(menu, parent);

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();

	// template name
	PedTemplatesCheat* pedTemplatesCheat = PedTemplatesCheat::GetInstance();
	const auto& templateNameOptions = pedTemplatesCheat->GetTemplatesList();
	if (!templateNameOptions.size()) {
		spdlog::warn("SpawnPedActionSegment: No ped templates available");
		return false;
	}

	auto* templateNameText = m_menuController->CreateItem<UiModule::Text>(m_vertCont, L"", options.textSize);
	m_templateNameController = m_menuController->CreateLatestItemController<UiModule::SelectController<std::wstring>>(
		templateNameText,
		templateNameOptions,
		templateNameOptions[0],
		UiModule::SelectControllerOptions{ L"Template: #", L"#" }
	);
	m_templateNameController->SetSaveCallback(std::bind(&SpawnPedActionSegment::OnTemplateControllerSave, this, std::placeholders::_1));

	// in nearest car
	auto* inNearestCarText = m_menuController->CreateItem<UiModule::Text>(m_vertCont, L"", options.textSize);
	m_inNearestCarController = m_menuController->CreateLatestItemController<UiModule::SelectController<bool>>(
		inNearestCarText,
		std::vector<bool>{ false, true },
		false,
		UiModule::SelectControllerOptions{ L"In nearest vehicle: #", L"#" }
	);
	m_inNearestCarController->SetConverter<YesNoConverter>();

	// sprite preview
	uiRoot->AddComponent<UiModule::Spacer>(m_vertCont, 0, options.menuSpacerHeight);
	UpdatePreview();
	SegmentSupport::AttachSegment(m_pedPreviewSegment, menu, m_vertCont);

	return true;
}

void ModMenuModule::SpawnPedActionSegment::Detach()
{
	m_templateNameController = nullptr;
	m_inNearestCarController = nullptr;
	SegmentSupport::DetachSegment(m_pedPreviewSegment);
	SegmentBase::DestroySegment();
}

void ModMenuModule::SpawnPedActionSegment::OnTemplateControllerSave(const std::wstring& templateName)
{
	UpdatePreview();
}

void ModMenuModule::SpawnPedActionSegment::UpdatePreview()
{
	if (!m_pedPreviewSegment || !m_templateNameController) return;

	PedTemplatesCheat* pedTemplatesCheat = PedTemplatesCheat::GetInstance();

	const std::wstring& pedTemplateName = m_templateNameController->GetValue().value();
	const auto* pedTemplate = pedTemplatesCheat->GetTemplate(pedTemplateName);

	if (!pedTemplate) {
		spdlog::warn(L"SpawnPedActionSegment: Cannot update preview, ped template \"{}\" not found.", pedTemplateName);
		return;
	}

	m_pedPreviewSegment->SetPedData(pedTemplate->data);
}
