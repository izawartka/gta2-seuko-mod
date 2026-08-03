#include "encoded-ped-id-segment.h"
#include "../root.h"

static const std::vector<std::wstring> typeOptions = {
	L"No character",
	L"Player main char.",
	L"Player current char.",
	L"Nearest char.",
	L"By char. ID",
};

ModMenuModule::EncodedPedIdSegment::EncodedPedIdSegment(const std::wstring& label, Utils::EncodedPedId value)
{
	m_label = label;
	m_value = value;
}

ModMenuModule::EncodedPedIdSegment::~EncodedPedIdSegment()
{

}

void ModMenuModule::EncodedPedIdSegment::SetSaveCallback(EncodedPedIdSegmentSaveCallback callback)
{
	m_saveCallback = callback;
}

void ModMenuModule::EncodedPedIdSegment::SetValue(Utils::EncodedPedId value)
{
	m_value = value;

	if (IsAttached()) {
		m_typeController->SetValue(GetTypeFromValue(m_value));
		UpdateCustomIdController();
	}
}

ModMenuModule::Utils::EncodedPedId ModMenuModule::EncodedPedIdSegment::GetValue() const
{
	return m_value;
}

bool ModMenuModule::EncodedPedIdSegment::Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent)
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
	m_typeController->SetSaveCallback(std::bind(&EncodedPedIdSegment::OnTypeControllerSave, this, std::placeholders::_1));
	m_typeMenuItemId = m_menuController->GetLatestMenuItemId();
	
	UpdateCustomIdController();

	return true;
}

void ModMenuModule::EncodedPedIdSegment::Detach()
{
	DestroyCustomIdController();
	DestroySegment();
}

const std::wstring& ModMenuModule::EncodedPedIdSegment::GetTypeFromValue(Utils::EncodedPedId value)
{
	switch (value) {
	case -1: return typeOptions[0]; // No char.
	case -2: return typeOptions[1]; // Player main char.
	case -3: return typeOptions[2]; // Player current char.
	case -4: return typeOptions[3]; // Nearest char.
	default: return typeOptions[4]; // By char. ID:
	}
}

ModMenuModule::Utils::EncodedPedId ModMenuModule::EncodedPedIdSegment::GetValueFromType(const std::wstring& type)
{
	if (type == typeOptions[0]) return -1; // No char.
	if (type == typeOptions[1]) return -2; // Player main char.
	if (type == typeOptions[2]) return -3; // Player current char.
	if (type == typeOptions[3]) return -4; // Nearest char.
	return 1;
}

void ModMenuModule::EncodedPedIdSegment::OnTypeControllerSave(const std::wstring& value)
{
	Utils::EncodedPedId newValue = GetValueFromType(value);
	if (newValue == 1 && m_customIdController) {
		return;
	}
	m_value = newValue;

	UpdateCustomIdController();

	if (m_saveCallback) m_saveCallback(m_value);
}

void ModMenuModule::EncodedPedIdSegment::OnCustomIdControllerSave(const int& value)
{
	m_value = value;

	if (m_saveCallback) m_saveCallback(m_value);
}

void ModMenuModule::EncodedPedIdSegment::DestroyCustomIdController()
{
	if (!m_customIdController) return;

	m_menuController->DeleteItemController(m_customIdMenuItemId);
	m_menuController->DeleteItem(m_customIdMenuItemId);
	m_customIdMenuItemId = -1;
	m_customIdController = nullptr;
}

void ModMenuModule::EncodedPedIdSegment::UpdateCustomIdController()
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
	m_customIdController->SetSaveCallback(std::bind(&EncodedPedIdSegment::OnCustomIdControllerSave, this, std::placeholders::_1));
	m_customIdController->SetClampCallback([](const int& value) {
		return std::max(0, value);
	});
	m_customIdMenuItemId = m_menuController->GetLatestMenuItemId();

	m_menuController->SetCurrentGroupId(prevGroupId);
}
