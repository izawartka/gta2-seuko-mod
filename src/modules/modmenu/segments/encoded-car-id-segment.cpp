#include "encoded-car-id-segment.h"
#include "../root.h"

static const std::vector<std::wstring> typeOptions = {
	L"No vehicle",
	L"Player main char.'s vehicle",
	L"Player current char.'s vehicle",
	L"Player current/last vehicle",
	L"Nearest vehicle",
	L"By vehicle ID",
};

ModMenuModule::EncodedCarIdSegment::EncodedCarIdSegment(const std::wstring& label, Utils::EncodedCarId value)
{
	m_label = label;
	m_value = value;
}

ModMenuModule::EncodedCarIdSegment::~EncodedCarIdSegment()
{

}

void ModMenuModule::EncodedCarIdSegment::SetSaveCallback(EncodedCarIdSegmentSaveCallback callback)
{
	m_saveCallback = callback;
}

void ModMenuModule::EncodedCarIdSegment::SetValue(Utils::EncodedCarId value)
{
	m_value = value;

	if (IsAttached()) {
		m_typeController->SetValue(GetTypeFromValue(m_value));
		UpdateCustomIdController();
	}
}

ModMenuModule::Utils::EncodedCarId ModMenuModule::EncodedCarIdSegment::GetValue() const
{
	return m_value;
}

bool ModMenuModule::EncodedCarIdSegment::Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent)
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
	m_typeController->SetSaveCallback(std::bind(&EncodedCarIdSegment::OnTypeControllerSave, this, std::placeholders::_1));
	m_typeMenuItemId = m_menuController->GetLatestMenuItemId();

	UpdateCustomIdController();

	return true;
}

void ModMenuModule::EncodedCarIdSegment::Detach()
{
	DestroyCustomIdController();
	DestroySegment();
}

const std::wstring& ModMenuModule::EncodedCarIdSegment::GetTypeFromValue(Utils::EncodedCarId value)
{
	switch (value) {
	case -1: return typeOptions[0]; // No vehicle
	case -2: return typeOptions[1]; // Player main char.'s vehicle
	case -3: return typeOptions[2]; // Player current char.'s vehicle
	case -4: return typeOptions[3]; // Player current/last vehicle
	case -5: return typeOptions[4]; // Nearest vehicle
	default: return typeOptions[5]; // By vehicle ID
	}
}

ModMenuModule::Utils::EncodedCarId ModMenuModule::EncodedCarIdSegment::GetValueFromType(const std::wstring& type)
{
	if (type == typeOptions[0]) return -1; // No vehicle
	if (type == typeOptions[1]) return -2; // Player main char.'s vehicle
	if (type == typeOptions[2]) return -3; // Player current char.'s vehicle
	if (type == typeOptions[3]) return -4; // Player current/last vehicle
	if (type == typeOptions[4]) return -5; // Nearest vehicle
	return 1;
}

void ModMenuModule::EncodedCarIdSegment::OnTypeControllerSave(const std::wstring& value)
{
	Utils::EncodedCarId newValue = GetValueFromType(value);
	if (newValue == 1 && m_customIdController) {
		return;
	}
	m_value = newValue;

	UpdateCustomIdController();

	if (m_saveCallback) m_saveCallback(m_value);
}

void ModMenuModule::EncodedCarIdSegment::OnCustomIdControllerSave(const int& value)
{
	m_value = value;

	if (m_saveCallback) m_saveCallback(m_value);
}

void ModMenuModule::EncodedCarIdSegment::DestroyCustomIdController()
{
	if (!m_customIdController) return;

	m_menuController->DeleteItemController(m_customIdMenuItemId);
	m_menuController->DeleteItem(m_customIdMenuItemId);
	m_customIdMenuItemId = -1;
	m_customIdController = nullptr;
}

void ModMenuModule::EncodedCarIdSegment::UpdateCustomIdController()
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
	m_customIdController->SetSaveCallback(std::bind(&EncodedCarIdSegment::OnCustomIdControllerSave, this, std::placeholders::_1));
	m_customIdController->SetClampCallback([](const int& value) {
		return std::max(0, value);
	});
	m_customIdMenuItemId = m_menuController->GetLatestMenuItemId();

	m_menuController->SetCurrentGroupId(prevGroupId);
}
