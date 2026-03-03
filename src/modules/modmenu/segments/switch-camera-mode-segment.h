#pragma once
#include "../common.h"
#include "../segment-base.h"
#include "../utils/camera-easy-mode-utils.h"

namespace ModMenuModule {
	struct SwitchCameraModeSegmentData {
		std::vector<Utils::CameraEasyMode::CameraEasyMode> selectedModes; // empty if all
	};

	class SwitchCameraModeSegment : public Segment<SwitchCameraModeSegmentData> {
	public:
		SwitchCameraModeSegment() = default;
		virtual ~SwitchCameraModeSegment() = default;

		virtual bool ValidateSegment() const override;

		virtual std::optional<SwitchCameraModeSegmentData> GetSegmentData() const override;
		virtual bool SetSegmentData(const SwitchCameraModeSegmentData& data) override;

		bool OnPassedMenuAction(UiModule::Selectable* item, UiModule::MenuItemId id) override;

	private:
		virtual bool Attach(ModMenuModule::MenuBase* menu, UiModule::Component* parent) override;
		virtual void Detach() override;

		void UpdateSelectedModesText();
		void CreateSelectedModesItem();
		void DestroySelectedModesItem();
		void UpdateSelectedModesItem();

		UiModule::SelectController<bool>* m_selectAllController = nullptr;
		UiModule::MenuItemId m_selectAllMenuItemId = -1;
		UiModule::Text* m_selectedModesText = nullptr;
		UiModule::MenuItemId m_selectedModesMenuItemId = -1;
		std::shared_ptr<std::vector<Utils::CameraEasyMode::CameraEasyMode>> m_selectedModes = std::make_shared<std::vector<Utils::CameraEasyMode::CameraEasyMode>>();
		bool m_lastSelectAllValue = true;
	};
}
