#include "toggle-native-cheat.h"
#include "../../../converters/native-cheat.h"
#include "../../../converters/native-cheat-state.h"
#include "../utils/toggle-native-cheat-utils.h"
#include "../toast-manager.h"
#include "../quick-action-registry.h"

static const std::string typeId = "ModMenu_ToggleNativeCheat";
static const std::wstring typeLabel = L"Toggle native cheat";

ModMenuModule::ToggleNativeCheatAction::ToggleNativeCheatAction()
{
	m_label = typeLabel;
}

ModMenuModule::ToggleNativeCheatAction::~ToggleNativeCheatAction()
{
}

const std::string& ModMenuModule::ToggleNativeCheatAction::GetTypeId()
{
	return typeId;
}

const std::wstring& ModMenuModule::ToggleNativeCheatAction::GetTypeLabel()
{
	return typeLabel;
}

ModMenuModule::ToggleNativeCheatSegment* ModMenuModule::ToggleNativeCheatAction::CreateSegmentInstance()
{
	return new ToggleNativeCheatSegment();
}

void ModMenuModule::ToggleNativeCheatAction::Execute()
{
	if (!m_data.has_value()) {
		spdlog::error("ToggleNativeCheatAction::Execute: No data to execute action.");
		return;
	}

	ToggleNativeCheatSegmentData data = m_data.value();
	std::wstring cheatName = NativeCheatConverter::ConvertToString(data.cheatAddress);

	auto result = Utils::ToggleNativeCheat::ToggleNativeCheat(data.cheatAddress, data.mode);

	if (!result.has_value()) {
		ModMenuModule::ToastManager::GetInstance()->Show({ L"Failed to toggle " + cheatName, ToastType::Error });
		return;
	}

	NativeCheatState newState = result.value();
	std::wstring stateStr = NativeCheatStateConverter<true>::ConvertToString(newState);
	ModMenuModule::ToastManager::GetInstance()->Show({ L"Set " + cheatName + L" to " + stateStr });
}

const std::wstring& ModMenuModule::ToggleNativeCheatAction::GetLabel() const
{
	return m_label;
}

void ModMenuModule::ToggleNativeCheatAction::OnDataChange()
{
	if (m_data.has_value()) {
		m_label = L"Toggle " + NativeCheatConverter::ConvertToString(m_data->cheatAddress);
	}
	else {
		m_label = typeLabel;
	}
}

REGISTER_QUICK_ACTION(ToggleNativeCheatAction)
