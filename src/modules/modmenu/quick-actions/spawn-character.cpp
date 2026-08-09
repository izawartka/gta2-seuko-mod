#include "spawn-character.h"
#include "../cheats/ped-templates.h"
#include "../toast-manager.h"
#include "../utils/spawn-ped-utils.h"
#include "../quick-action-registry.h"

static const std::string typeId = "ModMenu_SpawnCharacter";
static const std::wstring typeLabel = L"Spawn character";

ModMenuModule::SpawnCharacterAction::SpawnCharacterAction()
{
	m_label = typeLabel;
}

ModMenuModule::SpawnCharacterAction::~SpawnCharacterAction()
{}

const std::string& ModMenuModule::SpawnCharacterAction::GetTypeId()
{
	return typeId;
}

const std::wstring& ModMenuModule::SpawnCharacterAction::GetTypeLabel()
{
	return typeLabel;
}

ModMenuModule::SpawnPedActionSegment* ModMenuModule::SpawnCharacterAction::CreateSegmentInstance()
{
	return new SpawnPedActionSegment();
}

void ModMenuModule::SpawnCharacterAction::Execute()
{
	if (!m_data.has_value()) {
		spdlog::error("SpawnCharacterAction::Execute: No data to execute action.");
		return;
	}

	PedTemplatesCheat* pedTemplatesCheat = PedTemplatesCheat::GetInstance();
	if (!pedTemplatesCheat->IsEnabled()) {
		spdlog::error("SpawnCharacterAction::Execute: PedTemplatesCheat is not enabled.");
		return;
	}

	const SpawnPedActionSegmentData& data = m_data.value();

	const PedTemplate* pedTemplate = pedTemplatesCheat->GetTemplate(data.pedTemplateName);
	if (!pedTemplate) {
		ToastManager::GetInstance()->Show({ L"Failed to spawn " + data.pedTemplateName + L",", ToastType::Error});
		ToastManager::GetInstance()->Show({ L"Character template not found", ToastType::Error });
		return;
	}

	if (Utils::SpawnPed::SpawnPedAtPlayer(pedTemplate->data, data.inNearestCar)) {
		ModMenuModule::ToastManager::GetInstance()->Show({ L"Spawned " + data.pedTemplateName });
	}
	else {
		ModMenuModule::ToastManager::GetInstance()->Show({ L"Failed to spawn " + data.pedTemplateName, ToastType::Error });
	}
}

const std::wstring& ModMenuModule::SpawnCharacterAction::GetLabel() const
{
	return m_label;
}

std::vector<uint8_t> ModMenuModule::SpawnCharacterAction::SerializeData() const
{
	if (!m_data.has_value()) {
		spdlog::error("SpawnCharacterAction::SerializeData: No data to serialize.");
		return {};
	}

	const SpawnPedActionSegmentData& data = m_data.value();
	std::vector<uint8_t> buffer;
	size_t bufferSize = data.pedTemplateName.size() * sizeof(wchar_t) + 1;
	buffer.resize(bufferSize);
	std::memcpy(buffer.data(), data.pedTemplateName.data(), bufferSize);
	buffer[buffer.size() - 1] = data.inNearestCar ? 1 : 0;
	return buffer;
}

bool ModMenuModule::SpawnCharacterAction::DeserializeData(const std::vector<uint8_t>& data)
{
	if ((data.size() - 1) % sizeof(wchar_t) != 0) {
		spdlog::error("SpawnCharacterAction::DeserializeData: Invalid data size.");
		return false;
	}
	size_t wcharCount = (data.size() - 1) / sizeof(wchar_t);
	std::wstring pedTemplateName(wcharCount, L'\0');
	std::memcpy(&pedTemplateName[0], data.data(), data.size() - 1);
	bool inNearestCar = data[data.size() - 1] != 0;
	m_data = SpawnPedActionSegmentData{ pedTemplateName, inNearestCar };
	OnDataChange();
	return true;
}

void ModMenuModule::SpawnCharacterAction::OnDataChange()
{
	if (m_data.has_value()) {
		m_label = L"Spawn " + m_data->pedTemplateName;
	}
	else {
		m_label = typeLabel;
	}
}

REGISTER_QUICK_ACTION(SpawnCharacterAction)
