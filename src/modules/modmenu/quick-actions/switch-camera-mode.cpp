#include "switch-camera-mode.h"
#include "../../../converters/camera-easy-mode.h"
#include "../utils/camera-easy-mode-utils.h"
#include "../toast-manager.h"
#include "../quick-action-registry.h"

static const std::string typeId = "ModMenu_SwitchCameraMode";
static const std::wstring typeLabel = L"Switch camera mode";

ModMenuModule::SwitchCameraModeAction::SwitchCameraModeAction()
{
}

ModMenuModule::SwitchCameraModeAction::~SwitchCameraModeAction()
{
}

const std::string& ModMenuModule::SwitchCameraModeAction::GetTypeId()
{
	return typeId;
}

const std::wstring& ModMenuModule::SwitchCameraModeAction::GetTypeLabel()
{
	return typeLabel;
}

ModMenuModule::SwitchCameraModeSegment* ModMenuModule::SwitchCameraModeAction::CreateSegmentInstance()
{
	return new SwitchCameraModeSegment();
}

void ModMenuModule::SwitchCameraModeAction::Execute()
{
	if (!m_data.has_value()) {
		spdlog::error("SwitchCameraModeAction::Execute: No data to execute.");
		return;
	}

	const auto& selectedModes = m_data->selectedModes;
	Utils::CameraEasyMode::CameraEasyMode nextMode = Utils::CameraEasyMode::GetNextMode(
		selectedModes.empty() ? Utils::CameraEasyMode::GetSelectableModes() : selectedModes
	);
	std::wstring nextModeName = CameraEasyModeConverter::ConvertToString(nextMode);

	if (ModMenuModule::Utils::CameraEasyMode::SetCurrentMode(nextMode)) {
		ToastManager::GetInstance()->Show({ L"Switched camera mode to " + nextModeName, ToastType::Info });
	}
	else {
		ToastManager::GetInstance()->Show({ L"Failed to switch camera mode to " + nextModeName, ToastType::Error });
	}
}

const std::wstring& ModMenuModule::SwitchCameraModeAction::GetLabel() const
{
	return typeLabel;
}

std::vector<uint8_t> ModMenuModule::SwitchCameraModeAction::SerializeData() const
{
	if (!m_data.has_value()) {
		spdlog::error("SwitchCameraModeAction::SerializeData: No data to serialize.");
		return {};
	}
	const SwitchCameraModeSegmentData& data = m_data.value();
	std::vector<uint8_t> buffer;
	size_t selectedModesCount = data.selectedModes.size();
	buffer.resize(sizeof(size_t) + sizeof(Utils::CameraEasyMode::CameraEasyMode) * selectedModesCount);
	std::memcpy(buffer.data(), &selectedModesCount, sizeof(size_t));
	for (size_t i = 0; i < selectedModesCount; i++) {
		Utils::CameraEasyMode::CameraEasyMode mode = data.selectedModes[i];
		std::memcpy(buffer.data() + sizeof(size_t) + sizeof(Utils::CameraEasyMode::CameraEasyMode) * i, &mode, sizeof(Utils::CameraEasyMode::CameraEasyMode));
	}
	return buffer;
}

bool ModMenuModule::SwitchCameraModeAction::DeserializeData(const std::vector<uint8_t>& data)
{
	if (data.size() < sizeof(size_t)) {
		spdlog::error("QuickActionWithSegment::DeserializeData: Invalid data size.");
		return false;
	}
	size_t selectedModesCount;
	std::memcpy(&selectedModesCount, data.data(), sizeof(size_t));

	if (data.size() != sizeof(size_t) + sizeof(Utils::CameraEasyMode::CameraEasyMode) * selectedModesCount) {
		spdlog::error("QuickActionWithSegment::DeserializeData: Invalid data size for selected modes count.");
		return false;
	}

	std::vector<Utils::CameraEasyMode::CameraEasyMode> selectedModes;
	for (size_t i = 0; i < selectedModesCount; i++) {
		Utils::CameraEasyMode::CameraEasyMode mode;
		std::memcpy(&mode, data.data() + sizeof(size_t) + sizeof(Utils::CameraEasyMode::CameraEasyMode) * i, sizeof(Utils::CameraEasyMode::CameraEasyMode));

		// backwards compatibility
		if (mode == Utils::CameraEasyMode::CameraEasyMode::Unmodified2) {
			mode = Utils::CameraEasyMode::CameraEasyMode::Unmodified;
		}

		selectedModes.push_back(mode);
	}

	SwitchCameraModeSegmentData dataValue{ selectedModes };
	m_data = dataValue;
	OnDataChange();
	return true;
}

REGISTER_QUICK_ACTION(SwitchCameraModeAction)
