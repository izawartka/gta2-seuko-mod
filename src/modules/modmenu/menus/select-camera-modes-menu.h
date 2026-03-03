#pragma once
#include "../common.h"
#include "../menu-base.h"
#include "../utils/camera-easy-mode-utils.h"

namespace ModMenuModule {
	class SelectCameraModesMenu : public MenuBase {
	public:
		SelectCameraModesMenu(std::shared_ptr<std::vector<Utils::CameraEasyMode::CameraEasyMode>> selectedModes);
		virtual ~SelectCameraModesMenu();

	private:
		virtual bool Attach() override;

		void OnMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;

		void ToggleModeSelection(Utils::CameraEasyMode::CameraEasyMode mode);
		
		static std::wstring GetModeText(Utils::CameraEasyMode::CameraEasyMode mode, bool selected);

		std::shared_ptr<std::vector<Utils::CameraEasyMode::CameraEasyMode>> m_selectedModes;
		std::unordered_map<Utils::CameraEasyMode::CameraEasyMode, UiModule::Text*> m_modeToTextMap;
	};
}
