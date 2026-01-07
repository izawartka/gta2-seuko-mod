#include "spawn-object.h"
#include "../utils/spawn-object-at-player.h"
#include "../../../converters/object-type.h"
#include "../toast-manager.h"
#include "../quick-action-registry.h"

static const std::string typeId = "ModMenu_SpawnObject";
static const std::wstring typeLabel = L"Spawn object";

ModMenuModule::SpawnObjectAction::SpawnObjectAction()
{
	m_label = typeLabel;
}

ModMenuModule::SpawnObjectAction::~SpawnObjectAction()
{
}

const std::string& ModMenuModule::SpawnObjectAction::GetTypeId()
{
	return typeId;
}

const std::wstring& ModMenuModule::SpawnObjectAction::GetTypeLabel()
{
	return typeLabel;
}

ModMenuModule::SpawnObjectSegment* ModMenuModule::SpawnObjectAction::CreateSegmentInstance()
{
	return new SpawnObjectSegment();
}

void ModMenuModule::SpawnObjectAction::Execute()
{
	if (!m_data.has_value()) {
		spdlog::error("SpawnObjectAction::Execute: No data to execute action.");
		return;
	}

	SpawnObjectSegmentData data = m_data.value();
	std::wstring objectTypeStr = ObjectTypeConverter::ConvertToString(data.objectType);

	if (ModMenuModule::Utils::SpawnObjectAtPlayer(data.objectType)) {
		ModMenuModule::ToastManager::GetInstance()->Show({ L"Spawned " + objectTypeStr });
	}
	else {
		ModMenuModule::ToastManager::GetInstance()->Show({ L"Failed to spawn " + objectTypeStr, ToastType::Error });
	}
}

const std::wstring& ModMenuModule::SpawnObjectAction::GetLabel() const
{
	return m_label;
}

void ModMenuModule::SpawnObjectAction::OnDataChange()
{
	if (m_data.has_value()) {
		m_label = L"Spawn " + ObjectTypeConverter::ConvertToString(m_data->objectType);
	}
	else {
		m_label = typeLabel;
	}
}

REGISTER_QUICK_ACTION(SpawnObjectAction)
