#include "switch-camera-mode-segment.h"
#include "../toast-manager.h"
#include "../../../converters/all-specified.h"
#include "../../../converters/elipsis.h"
#include "../../../converters/camera-easy-modes-list.h"
#include "../menus/select-camera-modes-menu.h"
#include "../root.h"

bool ModMenuModule::SwitchCameraModeSegment::ValidateSegment() const
{
	if (!m_selectAllController) {
		spdlog::error("Cannot validate segment: controller is not initialized");
		return false;
	}

	bool selectAll = m_selectAllController->GetValue().value_or(false);
	if (selectAll) return true;

	if (m_selectedModes->size() < 2) {
		ToastManager::GetInstance()->Show({ L"At least two camera modes must be selected", ToastType::Warning });
		return false;
	}

	return true;
}

std::optional<ModMenuModule::SwitchCameraModeSegmentData> ModMenuModule::SwitchCameraModeSegment::GetSegmentData() const
{
	if (!m_selectAllController) {
		spdlog::error("Cannot get segment data: controller is not initialized");
		return std::nullopt;
	}

	bool selectAll = m_selectAllController->GetValue().value_or(true);
	auto selectedModes = selectAll 
		? std::vector<Utils::CameraEasyMode::CameraEasyMode>{} 
		: *m_selectedModes;

	return SwitchCameraModeSegmentData{
		selectedModes
	};
}

bool ModMenuModule::SwitchCameraModeSegment::SetSegmentData(const SwitchCameraModeSegmentData& data)
{
	if (!m_selectAllController) {
		spdlog::error("Cannot set segment data: controller is not initialized");
		return false;
	}

	bool selectAll = data.selectedModes.empty();
	m_selectAllController->SetValue(selectAll);

	if (!selectAll) {
		*m_selectedModes = data.selectedModes;
		UpdateSelectedModesItem();
	}

	return true;
}

bool ModMenuModule::SwitchCameraModeSegment::OnPassedMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	if (id == m_selectedModesMenuItemId) {
		MenuManager::GetInstance()->AddMenu<SelectCameraModesMenu>(m_selectedModes);
		return true;
	}

	return false;
}

bool ModMenuModule::SwitchCameraModeSegment::Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent)
{
	CreateSegment(menu, parent);

	UiModule::RootModule* uiRoot = UiModule::RootModule::GetInstance();
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	// select all controller
	UiModule::Text* selectAllText = m_menuController->CreateItem<UiModule::Text>(m_vertCont, L"", options.textSize);
	m_selectAllController = m_menuController->CreateLatestItemController<UiModule::SelectController<bool>>(
		selectAllText,
		std::vector<bool>{ true, false },
		m_lastSelectAllValue,
		UiModule::SelectControllerOptions{ L"Camera modes: #", L"#" }
	);
	m_selectAllController->SetConverter<AllSpecifiedConverter>();
	m_selectAllController->SetSaveCallback([this](bool selectAll) {
		UpdateSelectedModesItem();
	});
	m_selectAllMenuItemId = m_menuController->GetLatestMenuItemId();

	UpdateSelectedModesItem();

	return true;
}

void ModMenuModule::SwitchCameraModeSegment::Detach()
{
	if(m_selectAllController) {
		// save select all value to restore it after re-attaching the segment when going back from the submenu
		m_lastSelectAllValue = m_selectAllController->GetValue().value_or(true);
	}

	DestroySelectedModesItem();
	DestroySegment();

	m_selectAllMenuItemId = -1;
}

void ModMenuModule::SwitchCameraModeSegment::UpdateSelectedModesText()
{
	if (m_selectedModesText == nullptr) return;

	std::wstring truncatedModesListText = ElipsisConverter<
		std::vector<Utils::CameraEasyMode::CameraEasyMode>,
		CameraEasyModesListConverter
	>::ConvertToString(*m_selectedModes);

	std::wstring text = L"Selected modes: #" + truncatedModesListText + L"#";
	m_selectedModesText->SetText(text);
}

void ModMenuModule::SwitchCameraModeSegment::CreateSelectedModesItem()
{
	if (m_selectedModesText != nullptr) return;

	size_t selectAllIndex = m_menuController->GetItemIndex(m_selectAllMenuItemId);
	if (selectAllIndex == -1) {
		spdlog::error("Cannot create selected modes text: select all menu item not found");
		return;
	}

	m_menuController->SetNextAddedItemIndex(selectAllIndex + 1);
	m_selectedModesText = m_menuController->CreateItem<UiModule::Text>(m_vertCont, L"", ModMenuModule::RootModule::GetInstance()->GetOptions().textSize);
	m_selectedModesMenuItemId = m_menuController->GetLatestMenuItemId();

	UpdateSelectedModesText();
}

void ModMenuModule::SwitchCameraModeSegment::DestroySelectedModesItem()
{
	if (m_selectedModesText == nullptr) return;
	m_menuController->DeleteItem(m_selectedModesMenuItemId);
	m_selectedModesText = nullptr;
	m_selectedModesMenuItemId = -1;
}

void ModMenuModule::SwitchCameraModeSegment::UpdateSelectedModesItem()
{
	if(m_selectAllController == nullptr) {
		spdlog::error("Cannot update selected modes text: select all controller is not initialized");
		return;
	}

	bool selectAll = m_selectAllController->GetValue().value_or(true);

	if(selectAll) {
		DestroySelectedModesItem();
	}
	else {
		CreateSelectedModesItem();
	}
}
