#include "get-powerup.h"
#include "../utils/give-player-powerup.h"
#include "../../../converters/powerup.h"
#include "../toast-manager.h"
#include "../quick-action-registry.h"

static const std::string typeId = "ModMenu_GetPowerup";
static const std::wstring typeLabel = L"Get power-up";

ModMenuModule::GetPowerupAction::GetPowerupAction()
{
	m_label = typeLabel;
}

ModMenuModule::GetPowerupAction::~GetPowerupAction()
{
}

const std::string& ModMenuModule::GetPowerupAction::GetTypeId()
{
	return typeId;
}

const std::wstring& ModMenuModule::GetPowerupAction::GetTypeLabel()
{
	return typeLabel;
}

ModMenuModule::GetPowerupSegment* ModMenuModule::GetPowerupAction::CreateSegmentInstance()
{
	return new GetPowerupSegment();
}

void ModMenuModule::GetPowerupAction::Execute()
{
	if (!m_data.has_value()) {
		spdlog::error("GetPowerupAction::Execute: No data to execute action.");
		return;
	}

	GetPowerupSegmentData data = m_data.value();
	std::wstring powerupTypeStr = PowerupConverter::ConvertToString(data.powerupType);

	if (ModMenuModule::Utils::GivePlayerPowerup(data.powerupType)) {
		ModMenuModule::ToastManager::GetInstance()->Show({ L"Got " + powerupTypeStr });
	}
	else {
		ModMenuModule::ToastManager::GetInstance()->Show({ L"Failed to give " + powerupTypeStr, ToastType::Error });
	}
}

const std::wstring& ModMenuModule::GetPowerupAction::GetLabel() const
{
	return m_label;
}

void ModMenuModule::GetPowerupAction::OnDataChange()
{
	if (m_data.has_value()) {
		m_label = L"Give " + PowerupConverter::ConvertToString(m_data->powerupType);
	}
	else {
		m_label = typeLabel;
	}
}

REGISTER_QUICK_ACTION(GetPowerupAction)
