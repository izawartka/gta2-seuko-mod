#include "teleport.h"
#include "../cheats/player-pos.h"
#include "../../../converters/scr-vector3-rot.h"
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

ModMenuModule::PositionRotationSegment* ModMenuModule::TeleportAction::CreateSegmentInstance()
{
	return new PositionRotationSegment();
}

void ModMenuModule::TeleportAction::Execute()
{
	if (!m_data.has_value()) {
		spdlog::error("TeleportAction::Execute: No data to execute action.");
		return;
	}

	TeleportActionData data = m_data.value();

	PlayerPosCheat* playerPosCheat = PlayerPosCheat::GetInstance();
	if(!playerPosCheat->IsEnabled()) {
		spdlog::error("TeleportAction::Execute: PlayerPosCheat is not enabled.");
		return;
	}

	playerPosCheat->Teleport(data.position, [data](bool success) {
		if (success) {
			short* rotationPtr = Game::Utils::GetPlayerPedRotationPtr();
			if(rotationPtr) *rotationPtr = data.rotation;

			std::wstring positionStr = ScrVector3RotConverter::ConvertToString({data.position, data.rotation});
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

std::optional<ModMenuModule::TeleportActionData> ModMenuModule::TeleportAction::SegmentDataToData(const PositionRotationSegmentData& segmentData)
{
	return TeleportActionData{
		segmentData.position,
		segmentData.rotation
	};
}

std::optional<ModMenuModule::PositionRotationSegmentData> ModMenuModule::TeleportAction::DataToSegmentData(const TeleportActionData& data)
{
	return PositionRotationSegmentData{
		false,
		data.position,
		false,
		data.rotation
	};
}

bool ModMenuModule::TeleportAction::HandleInvalidDataSize(std::vector<uint8_t>& data)
{
	// Teleport action used to not include rotation
	if (data.size() == sizeof(Game::SCR_Vector3)) {
		TeleportActionData newData;
		newData.rotation = 0;
		memcpy(&newData.position, data.data(), sizeof(Game::SCR_Vector3));
		data.resize(sizeof(TeleportActionData));
		memcpy(data.data(), &newData, sizeof(TeleportActionData));

		return true;
	}

	return false;
}

void ModMenuModule::TeleportAction::OnDataChange()
{
	if (m_data.has_value()) {
		m_label = L"Teleport to " + ScrVector3RotConverter::ConvertToString({m_data.value().position, m_data.value().rotation});
	}
	else {
		m_label = typeLabel;
	}
}

REGISTER_QUICK_ACTION(TeleportAction)
