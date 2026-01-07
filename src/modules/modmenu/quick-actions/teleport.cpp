#include "teleport.h"
#include "../cheats/player-pos.h"
#include "../../../converters/scr-vector3.h"
#include "../toast-manager.h"
#include "../quick-action-registry.h"

static const std::string typeId = "ModMenu_Teleport";
static const std::wstring typeLabel = L"Teleport";

ModMenuModule::TeleportAction::TeleportAction()
{
	m_label = typeLabel;
}

ModMenuModule::TeleportAction::~TeleportAction()
{
}

const std::string& ModMenuModule::TeleportAction::GetTypeId()
{
	return typeId;
}

const std::wstring& ModMenuModule::TeleportAction::GetTypeLabel()
{
	return typeLabel;
}

ModMenuModule::PositionSegment* ModMenuModule::TeleportAction::CreateSegmentInstance()
{
	return new PositionSegment();
}

void ModMenuModule::TeleportAction::Execute()
{
	if (!m_data.has_value()) {
		spdlog::error("TeleportAction::Execute: No data to execute action.");
		return;
	}

	Game::SCR_Vector3 data = m_data.value();

	PlayerPosCheat* playerPosCheat = PlayerPosCheat::GetInstance();
	if(!playerPosCheat->IsEnabled()) {
		spdlog::error("TeleportAction::Execute: PlayerPosCheat is not enabled.");
		return;
	}

	playerPosCheat->Teleport(data, [data](bool success) {
		if (success) {
			std::wstring positionStr = ScrVector3Converter::ConvertToString(data);
			ModMenuModule::ToastManager::GetInstance()->Show({ L"Teleported to " + positionStr });
		}
		else {
			ModMenuModule::ToastManager::GetInstance()->Show({ L"Teleport failed", ToastType::Error });
		}
	});
}

const std::wstring& ModMenuModule::TeleportAction::GetLabel() const
{
	return m_label;
}

std::optional<Game::SCR_Vector3> ModMenuModule::TeleportAction::SegmentDataToData(const PositionSegmentData& segmentData)
{
	return segmentData.position;
}

std::optional<ModMenuModule::PositionSegmentData> ModMenuModule::TeleportAction::DataToSegmentData(const Game::SCR_Vector3& data)
{
	return PositionSegmentData{
		false,
		data,
		false
	};
}

void ModMenuModule::TeleportAction::OnDataChange()
{
	if (m_data.has_value()) {
		m_label = L"Teleport to " + ScrVector3Converter::ConvertToString(m_data.value());
	}
	else {
		m_label = typeLabel;
	}
}

REGISTER_QUICK_ACTION(TeleportAction)
