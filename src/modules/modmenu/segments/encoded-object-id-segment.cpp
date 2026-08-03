#include "encoded-object-id-segment.h"
#include "../root.h"

static const std::vector<std::wstring> typeOptions = {
	L"No object",
	L"Nearest object",
	L"By object ID",
};

ModMenuModule::EncodedObjectIdSegment::EncodedObjectIdSegment(const std::wstring& label, Utils::EncodedObjectId value)
{
	m_label = label;
	m_value = value;
}

ModMenuModule::EncodedObjectIdSegment::~EncodedObjectIdSegment()
{

}

void ModMenuModule::EncodedObjectIdSegment::SetSaveCallback(EncodedObjectIdSegmentSaveCallback callback)
{
	m_saveCallback = callback;
}

void ModMenuModule::EncodedObjectIdSegment::SetValue(Utils::EncodedObjectId value)
{
	m_value = value;

	if (IsAttached()) {
		m_typeController->SetValue(GetTypeFromValue(m_value));
		UpdateCustomIdController();
	}
}

ModMenuModule::Utils::EncodedObjectId ModMenuModule::EncodedObjectIdSegment::GetValue() const
{
	return m_value;
}

bool ModMenuModule::EncodedObjectIdSegment::Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent)
{
	CreateSegment(menu, parent);

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	const std::wstring& selectedType = GetTypeFromValue(m_value);

	auto* typeText = m_menuController->CreateItem<UiModule::Text>(m_vertCont, L"", options.textSize);
	m_typeController = m_menuController->CreateLatestItemController<UiModule::SelectController<std::wstring>>(
		typeText,
		typeOptions,
		selectedType,
		UiModule::SelectControllerOptions{ (m_label + L": #"), L"#" }
	);
	m_typeController->SetSaveCallback(std::bind(&EncodedObjectIdSegment::OnTypeControllerSave, this, std::placeholders::_1));
	m_typeMenuItemId = m_menuController->GetLatestMenuItemId();

	UpdateCustomIdController();

	return true;
}

void ModMenuModule::EncodedObjectIdSegment::Detach()
{
	DestroyCustomIdController();
	DestroySegment();
}

const std::wstring& ModMenuModule::EncodedObjectIdSegment::GetTypeFromValue(Utils::EncodedObjectId value)
{
	switch (value) {
	case -1: return typeOptions[0]; // No object
	case -2: return typeOptions[1]; // Nearest object
	default: return typeOptions[2]; // By object ID
	}
}

ModMenuModule::Utils::EncodedObjectId ModMenuModule::EncodedObjectIdSegment::GetValueFromType(const std::wstring& type)
{
	if (type == typeOptions[0]) return -1; // No object
	if (type == typeOptions[1]) return -2; // Nearest object
	return 1;
}

void ModMenuModule::EncodedObjectIdSegment::OnTypeControllerSave(const std::wstring& value)
{
	Utils::EncodedObjectId newValue = GetValueFromType(value);
	if (newValue == 1 && m_customIdController) {
		return;
	}
	m_value = newValue;

	UpdateCustomIdController();

	if (m_saveCallback) m_saveCallback(m_value);
}

void ModMenuModule::EncodedObjectIdSegment::OnCustomIdControllerSave(const int& value)
{
	m_value = value;

	if (m_saveCallback) m_saveCallback(m_value);
}

void ModMenuModule::EncodedObjectIdSegment::DestroyCustomIdController()
{
	if (!m_customIdController) return;

	m_menuController->DeleteItemController(m_customIdMenuItemId);
	m_menuController->DeleteItem(m_customIdMenuItemId);
	m_customIdMenuItemId = -1;
	m_customIdController = nullptr;
}

void ModMenuModule::EncodedObjectIdSegment::UpdateCustomIdController()
{
	if (m_value < 0) {
		DestroyCustomIdController();
		return;
	}

	if (m_customIdController) {
		m_customIdController->SetValue(m_value);
		return;
	}

	auto prevGroupId = m_menuController->GetCurrentGroupId();
	m_menuController->SetCurrentGroupId(m_menuGroupId);

	size_t customIdIndex = m_menuController->GetItemIndex(m_typeMenuItemId) + 1;
	m_menuController->SetNextAddedItemIndex(customIdIndex);

	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();
	auto* customIdText = m_menuController->CreateItem<UiModule::Text>(m_vertCont, L"", options.textSize);
	m_customIdController = m_menuController->CreateLatestItemController<UiModule::EditableController<int>>(
		customIdText,
		m_value,
		UiModule::EditableControllerOptions{ (m_label + L" ID: #"), L"#" }
	);
	m_customIdController->SetSaveCallback(std::bind(&EncodedObjectIdSegment::OnCustomIdControllerSave, this, std::placeholders::_1));
	m_customIdController->SetClampCallback([](const int& value) {
		return std::max(0, value);
	});
	m_customIdMenuItemId = m_menuController->GetLatestMenuItemId();

	m_menuController->SetCurrentGroupId(prevGroupId);
}
