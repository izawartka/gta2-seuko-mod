#include "ped-templates.h"
#include "../../../game/game.h"
#include "ped-templates-builtin.cpp"
#include "../cheat-registry.h"

static constexpr uint8_t PERSISTENCE_VERSION = 1;

ModMenuModule::PedTemplatesCheat* ModMenuModule::PedTemplatesCheat::m_instance = nullptr;

ModMenuModule::PedTemplatesCheat::PedTemplatesCheat() : ModMenuModule::CheatBase("")
{
	assert(m_instance == nullptr && "PedTemplatesCheat instance already exists");
	m_instance = this;
}

ModMenuModule::PedTemplatesCheat::~PedTemplatesCheat()
{
	m_instance = nullptr;
}

ModMenuModule::PedTemplatesCheat* ModMenuModule::PedTemplatesCheat::GetInstance()
{
	assert(m_instance && "PedTemplatesCheat not initialized!");
	return m_instance;
}

bool ModMenuModule::PedTemplatesCheat::SaveTemplate(const std::wstring& name, const Utils::SpawnPed::SpawnPedData& data)
{
	if (name.empty()) {
		spdlog::warn("PedTemplatesCheat::SaveTemplate: Name cannot be empty");
		return false;
	}

	if (builtInPedTemplates.find(name) != builtInPedTemplates.end()) {
		spdlog::warn("PedTemplatesCheat::SaveTemplate: Cannot overwrite built-in template");
		return false;
	}

	m_templates[name] = PedTemplate{
		name,
		data,
		false,
		true
	};
	m_cacheDirty = true;

	spdlog::info("PedTemplatesCheat::SaveTemplate: Ped template saved");
	return true;
}

bool ModMenuModule::PedTemplatesCheat::SaveTemplate(TempPedTemplateId tempTemplateId)
{
	auto it = m_tempTemplates.find(tempTemplateId);
	if (it == m_tempTemplates.end()) {
		spdlog::warn("PedTemplatesCheat::GetTemplate: Temporary template {} not found", tempTemplateId);
		return false;
	}

	PedTemplate* tempTemplate = it->second.pedTemplatePtr.get();
	if (!SaveTemplate(tempTemplate->name, tempTemplate->data)) {
		return false;
	}

	tempTemplate->isBuiltin = false;
	tempTemplate->isSaved = true;
	return true;
}

bool ModMenuModule::PedTemplatesCheat::SaveTemplate(TempPedTemplateId tempTemplateId, const std::wstring& customName)
{
	auto it = m_tempTemplates.find(tempTemplateId);
	if (it == m_tempTemplates.end()) {
		spdlog::warn("PedTemplatesCheat::GetTemplate: Temporary template {} not found", tempTemplateId);
		return false;
	}

	PedTemplate* tempTemplate = it->second.pedTemplatePtr.get();
	if (!SaveTemplate(customName, tempTemplate->data)) {
		return false;
	}

	tempTemplate->name = customName;
	tempTemplate->isBuiltin = false;
	tempTemplate->isSaved = true;
	return true;
}

const ModMenuModule::PedTemplate* ModMenuModule::PedTemplatesCheat::GetTemplate(const std::wstring& name) const
{
	auto builtinIt = builtInPedTemplates.find(name);
	if (builtinIt != builtInPedTemplates.end()) {
		return &builtinIt->second;
	}

	auto it = m_templates.find(name);
	if (it == m_templates.end()) {
		return nullptr;
	}
	return &it->second;
}

bool ModMenuModule::PedTemplatesCheat::DeleteTemplate(const std::wstring& name)
{
	auto it = m_templates.find(name);
	if (it == m_templates.end()) {
		spdlog::warn("PedTemplatesCheat::DeleteTemplate: Template not found or built-in");
		return false;
	}
	m_templates.erase(it);
	m_cacheDirty = true;

	spdlog::info("PedTemplatesCheat::DeleteTemplate: Template deleted");
	return true;
}

bool ModMenuModule::PedTemplatesCheat::RenameTemplate(const std::wstring& oldName, const std::wstring& newName)
{
	auto it = m_templates.find(oldName);
	if (it == m_templates.end()) {
		spdlog::warn("PedTemplatesCheat::UpdateTemplate: Template not found or built-in");
		return false;
	}

	if (m_templates.find(newName) != m_templates.end() || builtInPedTemplates.find(newName) != builtInPedTemplates.end()) {
		spdlog::warn("PedTemplatesCheat::RenameTemplate: Ped template with that name already exists");
		return false;
	}

	m_templates.emplace(newName, PedTemplate{
		newName,
		it->second.data,
		false,
		true
	});
	m_templates.erase(it);
	m_cacheDirty = true;

	spdlog::info("PedTemplatesCheat::RenameTemplate: Template renamed");
	return true;
}

const std::vector<std::wstring>& ModMenuModule::PedTemplatesCheat::GetTemplatesList()
{
	RefreshSortedCache();
	return m_sortedCache;
}

ModMenuModule::TempPedTemplateId ModMenuModule::PedTemplatesCheat::FindOrCreateTempTemplate(const std::string& persistenceKey, const std::wstring& baseTemplateName)
{
	auto it = FindTempTemplateByPersistenceKey(persistenceKey);

	if (it != m_tempTemplates.end()) {
		spdlog::debug("PedTemplatesCheat::FindOrCreateTempTemplate: Using existing temporary template with persistence key {}", persistenceKey);
		return it->second.id;
	}

	spdlog::debug("PedTemplatesCheat::FindOrCreateTempTemplate: Creating temporary template with persistence key {}", persistenceKey);

	const PedTemplate* baseTemplate = GetTemplate(baseTemplateName);
	if (!baseTemplate) {
		spdlog::warn(L"PedTemplatesCheat::FindOrCreateTempTemplate: Base template \"{}\" not found", baseTemplateName);
		return -1;
	}

	TempPedTemplateId newId = m_nextTempTemplateId++;
	m_tempTemplates.emplace(newId, TempPedTemplate{
		persistenceKey,
		newId,
		std::make_shared<PedTemplate>(*baseTemplate)
	});

	return newId;
}

ModMenuModule::TempPedTemplateId ModMenuModule::PedTemplatesCheat::FindTempTemplate(const std::string& persistenceKey)
{
	auto it = FindTempTemplateByPersistenceKey(persistenceKey);

	if (it == m_tempTemplates.end()) {
		return -1;
	}

	return it->second.id;
}

std::shared_ptr<ModMenuModule::PedTemplate> ModMenuModule::PedTemplatesCheat::GetTempTemplate(TempPedTemplateId id) const
{
	auto it = m_tempTemplates.find(id);
	if (it == m_tempTemplates.end()) {
		return nullptr;
	}
	return it->second.pedTemplatePtr;
}

void ModMenuModule::PedTemplatesCheat::DeleteTempTemplate(TempPedTemplateId id)
{
	auto it = m_tempTemplates.find(id);
	if (it == m_tempTemplates.end()) {
		spdlog::warn("PedTemplatesCheat::DeleteTempTemplate: Temporary template {} not found", id);
		return;
	}

	m_tempTemplates.erase(it);
}

void ModMenuModule::PedTemplatesCheat::OnFirstEnable()
{
	LoadFromPersistence();
}

void ModMenuModule::PedTemplatesCheat::OnEnable()
{

}

void ModMenuModule::PedTemplatesCheat::OnDisable()
{
	SaveToPersistence();
}

std::unordered_map<ModMenuModule::TempPedTemplateId, ModMenuModule::PedTemplatesCheat::TempPedTemplate>::iterator 
ModMenuModule::PedTemplatesCheat::FindTempTemplateByPersistenceKey(const std::string& persistenceKey)
{
	if (persistenceKey.empty()) {
		return m_tempTemplates.end();
	}

	return std::find_if(m_tempTemplates.begin(), m_tempTemplates.end(), [&](const auto& pair) {
		return pair.second.persistenceKey == persistenceKey;
	});
}

void ModMenuModule::PedTemplatesCheat::SaveToPersistence() const
{
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();

	size_t dataSize = 1 + sizeof(size_t) * 2;

	for (const auto& pair : m_templates) {
		const PedTemplate& templateData = pair.second;
		dataSize += sizeof(size_t) + templateData.name.size() * sizeof(wchar_t);
		dataSize += sizeof(Utils::SpawnPed::SpawnPedData);
	}

	size_t saveableTempTemplateCount = 0;
	for (const auto& pair : m_tempTemplates) {
		if (pair.second.persistenceKey.empty()) continue;

		const TempPedTemplate& tempTemplate = pair.second;
		dataSize += sizeof(size_t) + tempTemplate.persistenceKey.size() * sizeof(char);
		dataSize += sizeof(size_t) + tempTemplate.pedTemplatePtr->name.size() * sizeof(wchar_t);
		dataSize += sizeof(Utils::SpawnPed::SpawnPedData);
		dataSize += sizeof(bool); // isBuiltin
		dataSize += sizeof(bool); // isSaved

		saveableTempTemplateCount++;
	}

	std::unique_ptr<uint8_t[]> dataPtr = std::make_unique<uint8_t[]>(dataSize);
	dataPtr[0] = PERSISTENCE_VERSION;
	size_t offset = 1;

	auto writeSizeT = [&dataPtr, &offset](size_t value) {
		memcpy(dataPtr.get() + offset, &value, sizeof(size_t));
		offset += sizeof(size_t);
	};

	auto writeWideString = [&dataPtr, &offset, writeSizeT](const std::wstring& str) {
		writeSizeT(str.size());
		memcpy(dataPtr.get() + offset, str.data(), str.size() * sizeof(wchar_t));
		offset += str.size() * sizeof(wchar_t);
	};

	auto writeString = [&dataPtr, &offset, writeSizeT](const std::string& str) {
		writeSizeT(str.size());
		memcpy(dataPtr.get() + offset, str.data(), str.size() * sizeof(char));
		offset += str.size() * sizeof(char);
	};

	auto writeSpawnPedData = [&dataPtr, &offset](const Utils::SpawnPed::SpawnPedData& data) {
		memcpy(dataPtr.get() + offset, &data, sizeof(Utils::SpawnPed::SpawnPedData));
		offset += sizeof(Utils::SpawnPed::SpawnPedData);
	};

	auto writeBool = [&dataPtr, &offset](bool value) {
		memcpy(dataPtr.get() + offset, &value, sizeof(bool));
		offset += sizeof(bool);
	};

	// templates
	writeSizeT(m_templates.size());
	for (const auto& pair : m_templates) {
		const PedTemplate& templateData = pair.second;
		writeWideString(templateData.name);
		writeSpawnPedData(templateData.data);
	}

	// temp templates
	writeSizeT(saveableTempTemplateCount);
	for (const auto& pair : m_tempTemplates) {
		const TempPedTemplate& tempTemplate = pair.second;
		if (tempTemplate.persistenceKey.empty()) continue;
		writeString(tempTemplate.persistenceKey);
		writeWideString(tempTemplate.pedTemplatePtr->name);
		writeSpawnPedData(tempTemplate.pedTemplatePtr->data);
		writeBool(tempTemplate.pedTemplatePtr->isBuiltin);
		writeBool(tempTemplate.pedTemplatePtr->isSaved);
	}

	persistence->SaveRaw("Cheat_PedTemplates_State", dataPtr.get(), dataSize);
}

void ModMenuModule::PedTemplatesCheat::LoadFromPersistence()
{
	m_templates.clear();
	m_tempTemplates.clear();
	m_nextTempTemplateId = 1;
	m_cacheDirty = true;

	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();
	std::unique_ptr<uint8_t[]> dataPtr = nullptr;
	size_t dataSize = 0;
	if (!persistence->LoadRaw("Cheat_PedTemplates_State", dataPtr, dataSize)) return;
	if (dataSize < 1) {
		spdlog::error("PedTemplatesCheat::LoadFromPersistence: invalid data size");
		return;
	}
	uint8_t version = dataPtr[0];
	if (version != PERSISTENCE_VERSION && !ConvertPersistence(dataPtr, dataSize, version)) {
		return;
	}
	size_t offset = 1;

	auto showDataSizeError = [dataSize, version]() {
		spdlog::error("PedTemplatesCheat::LoadFromPersistence: invalid data size {} for version {}", dataSize, version);
	};

	auto readSizeT = [&dataPtr, &offset, dataSize, showDataSizeError](size_t& out) {
		if (offset + sizeof(size_t) > dataSize) {
			showDataSizeError();
			return false;
		}
		memcpy(&out, dataPtr.get() + offset, sizeof(size_t));
		offset += sizeof(size_t);
		return true;
	};

	auto readWideString = [&dataPtr, &offset, dataSize, readSizeT, showDataSizeError](std::wstring& out) {
		size_t strSize = 0;
		if (!readSizeT(strSize)) return false;
		if (offset + strSize * sizeof(wchar_t) > dataSize) {
			showDataSizeError();
			return false;
		}
		out.assign(reinterpret_cast<wchar_t*>(dataPtr.get() + offset), strSize);
		offset += strSize * sizeof(wchar_t);
		return true;
	};

	auto readString = [&dataPtr, &offset, dataSize, readSizeT, showDataSizeError](std::string& out) {
		size_t strSize = 0;
		if (!readSizeT(strSize)) return false;
		if (offset + strSize * sizeof(char) > dataSize) {
			showDataSizeError();
			return false;
		}
		out.assign(reinterpret_cast<char*>(dataPtr.get() + offset), strSize);
		offset += strSize * sizeof(char);
		return true;
	};

	auto readSpawnPedData = [&dataPtr, &offset, dataSize, showDataSizeError](Utils::SpawnPed::SpawnPedData& out) {
		if (offset + sizeof(Utils::SpawnPed::SpawnPedData) > dataSize) {
			showDataSizeError();
			return false;
		}
		memcpy(&out, dataPtr.get() + offset, sizeof(Utils::SpawnPed::SpawnPedData));
		offset += sizeof(Utils::SpawnPed::SpawnPedData);
		return true;
	};

	auto readBool = [&dataPtr, &offset, dataSize, showDataSizeError](bool& out) {
		if (offset + sizeof(bool) > dataSize) {
			showDataSizeError();
			return false;
		}
		memcpy(&out, dataPtr.get() + offset, sizeof(bool));
		offset += sizeof(bool);
		return true;
	};

	size_t templateCount = 0;
	if (!readSizeT(templateCount)) return;

	for (size_t i = 0; i < templateCount; ++i) {
		PedTemplate templateData;
		if (!readWideString(templateData.name)) return;
		if (!readSpawnPedData(templateData.data)) return;
		templateData.isBuiltin = false;
		templateData.isSaved = true;
		m_templates[templateData.name] = templateData;
	}

	size_t tempTemplateCount = 0;
	if (!readSizeT(tempTemplateCount)) return;

	for (size_t i = 0; i < tempTemplateCount; ++i) {
		TempPedTemplate tempTemplate;
		if (!readString(tempTemplate.persistenceKey)) return;
		tempTemplate.id = m_nextTempTemplateId++;
		tempTemplate.pedTemplatePtr = std::make_shared<PedTemplate>();
		if (!readWideString(tempTemplate.pedTemplatePtr->name)) return;
		if (!readSpawnPedData(tempTemplate.pedTemplatePtr->data)) return;
		if (!readBool(tempTemplate.pedTemplatePtr->isBuiltin)) return;
		if (!readBool(tempTemplate.pedTemplatePtr->isSaved)) return;
		m_tempTemplates[tempTemplate.id] = tempTemplate;
	}

	spdlog::info("PedTemplatesCheat::LoadFromPersistence: loaded {} template(s) and {} temporary template(s)", templateCount, tempTemplateCount);
}

bool ModMenuModule::PedTemplatesCheat::ConvertPersistence(std::unique_ptr<uint8_t[]>& dataPtr, size_t& dataSize, uint8_t version)
{
	spdlog::error("PedTemplatesCheat::ConvertPersistence: unsupported version {}", version);
	return false;
}

void ModMenuModule::PedTemplatesCheat::RefreshSortedCache()
{
	if (!m_cacheDirty) return;
	m_sortedCache = builtInPedTemplateNames;
	m_sortedCache.reserve(m_sortedCache.size() + m_templates.size());
	for (const auto& pair : m_templates) {
		m_sortedCache.push_back(pair.first);
	}
	std::sort(m_sortedCache.begin(), m_sortedCache.end());
	m_cacheDirty = false;
}

REGISTER_CHEAT(PedTemplatesCheat)
