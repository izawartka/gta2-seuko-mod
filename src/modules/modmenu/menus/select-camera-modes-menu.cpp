#include "select-camera-modes-menu.h"
#include "../utils/camera-easy-mode-utils.h"
#include "../../../converters/camera-easy-mode.h"
#include "../root.h"

ModMenuModule::SelectCameraModesMenu::SelectCameraModesMenu(std::shared_ptr<std::vector<Utils::CameraEasyMode::CameraEasyMode>> selectedModes)
{
	m_selectedModes = selectedModes;
}

ModMenuModule::SelectCameraModesMenu::~SelectCameraModesMenu()
{

}

bool ModMenuModule::SelectCameraModesMenu::Attach()
{
	UiModule::Component* vertCont;
	CreateMenu(L"#Select camera modes#", vertCont);
	const auto& options = ModMenuModule::RootModule::GetInstance()->GetOptions();

	m_menuController->CreateItem<UiModule::Text>(vertCont, L"Go back", options.textSize);

	if(m_selectedModes == nullptr) {
		spdlog::error("SelectCameraModesMenu: selectedModes is nullptr");
		return false;
	}

	const auto& selectedModes = *m_selectedModes.get();

	const auto& selectableModes = Utils::CameraEasyMode::GetSelectableModes();
	for (const auto& mode : selectableModes) {
		bool isSelected = std::find(selectedModes.begin(), selectedModes.end(), mode) != selectedModes.end();
		std::wstring modeText = GetModeText(mode, isSelected);
		UiModule::Text* modeTextComponent = m_menuController->CreateItem<UiModule::Text>(vertCont, modeText, options.textSize);
		m_modeToTextMap[mode] = modeTextComponent;
	}

	SetPreviousSelectedIndex();

	return true;
}

void ModMenuModule::SelectCameraModesMenu::OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id)
{
	ModMenuModule::MenuManager* menuManager = ModMenuModule::MenuManager::GetInstance();

	if (id == 0) {
		menuManager->RemoveLastMenu();
		return;
	}

	const auto& selectableModes = Utils::CameraEasyMode::GetSelectableModes();
	size_t modeIndex = id - 1;
	if (modeIndex >= selectableModes.size()) return;

	Utils::CameraEasyMode::CameraEasyMode selectedMode = selectableModes[modeIndex];
	ToggleModeSelection(selectedMode);
}

void ModMenuModule::SelectCameraModesMenu::ToggleModeSelection(Utils::CameraEasyMode::CameraEasyMode mode)
{
	bool isSelected = false;

	if (m_selectedModes == nullptr) {
		spdlog::error("SelectCameraModesMenu: selectedModes is nullptr");
		return;
	}

	std::vector<Utils::CameraEasyMode::CameraEasyMode>& selectedModes = *m_selectedModes;
	auto it = std::find(selectedModes.begin(), selectedModes.end(), mode);
	if (it != selectedModes.end()) {
		selectedModes.erase(it);
		isSelected = false;
	} else {
		selectedModes.push_back(mode);
		isSelected = true;
	}

	UiModule::Text* modeText = m_modeToTextMap.at(mode);
	modeText->SetText(GetModeText(mode, isSelected));
}

std::wstring ModMenuModule::SelectCameraModesMenu::GetModeText(Utils::CameraEasyMode::CameraEasyMode mode, bool selected)
{
	return (selected ? L"#(x)# " : L"#( )# ") + CameraEasyModeConverter::ConvertToString(mode);
}
