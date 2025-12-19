#include "quick-action-manager.h"
#include "utils/add-default-actions.h"

static const std::wstring emptyLabel = L"";

ModMenuModule::QuickActionManager* ModMenuModule::QuickActionManager::m_instance = nullptr;

ModMenuModule::QuickActionManager* ModMenuModule::QuickActionManager::GetInstance() 
{
	assert(m_instance && "QuickActionManager not initialized!");
	return m_instance;
}

std::vector<ModMenuModule::QuickActionTypeIndex> ModMenuModule::QuickActionManager::GetAllTypes()
{
	std::vector<QuickActionTypeIndex> typeIndices;
	for (const auto& pair : QuickActionRegistry::Actions()) {
		typeIndices.push_back(pair.first);
	}
	return typeIndices;
}

const std::wstring& ModMenuModule::QuickActionManager::GetTypeLabel(QuickActionTypeIndex typeIndex)
{
	auto actionItem = QuickActionRegistry::GetByTypeIndex(typeIndex);
	if (actionItem == nullptr) {
		spdlog::error("GetTypeLabel: No quick action type found for index {}", std::type_index(typeIndex).name());
		return emptyLabel;
	}
	return actionItem->typeLabel;
}

bool ModMenuModule::QuickActionManager::HasSegmentFactory(QuickActionTypeIndex typeIndex)
{
	auto actionItem = QuickActionRegistry::GetByTypeIndex(typeIndex);
	if (actionItem == nullptr) {
		spdlog::error("HasSegmentFactory: No quick action type found for index {}", std::type_index(typeIndex).name());
		return false;
	}
	return actionItem->segmentFactory != nullptr;
}

ModMenuModule::SegmentBase* ModMenuModule::QuickActionManager::CreateSegment(QuickActionTypeIndex typeIndex)
{
	auto actionItem = QuickActionRegistry::GetByTypeIndex(typeIndex);
	if (actionItem == nullptr) {
		spdlog::error("CreateSegment: No quick action type found for index {}", std::type_index(typeIndex).name());
		return nullptr;
	}
	if (actionItem->segmentFactory == nullptr) {
		spdlog::error("CreateSegment: Action type '{}' has no segment factory", actionItem->typeId);
		return nullptr;
	}
	return actionItem->segmentFactory();
}

std::optional<ModMenuModule::QuickActionInfo> ModMenuModule::QuickActionManager::GetInfo(QuickActionId actionId) const
{
	QuickActionEntry* entry = const_cast<QuickActionManager*>(this)->GetQuickActionEntry(actionId);
	if (!entry || !entry->keyBind || !entry->action) {
		spdlog::error("GetInfo: No quick action found for ID {}", actionId);
		return std::nullopt;
	}

	ModMenuModule::QuickActionInfo info(
		*(entry->keyBind),
		entry->action->GetLabel(),
		entry->customLabel,
		entry->typeIndex
	);

	return info;
}

std::vector<ModMenuModule::QuickActionId> ModMenuModule::QuickActionManager::GetAll() const
{
	std::vector<QuickActionId> ids;
	for (const auto& pair : m_quickActions) {
		ids.push_back(pair.first);
	}
	return ids;
}

bool ModMenuModule::QuickActionManager::SetDataFromSegmentData(QuickActionId actionId, SegmentBase* segment)
{
	spdlog::debug("SetDataFromSegmentData: Setting data for quick action ID {}", actionId);

	QuickActionEntry* entry = GetQuickActionEntry(actionId);
	if (!entry) {
		spdlog::error("SetDataFromSegmentData: No quick action found for ID {}", actionId);
		return false;
	}

	entry->action->SetDataFromSegmentData(segment);
	return true;
}

bool ModMenuModule::QuickActionManager::SetSegmentDataFromData(QuickActionId actionId, SegmentBase* segment)
{
	spdlog::debug("SetSegmentDataFromData: Setting segment data for quick action ID {}", actionId);

	QuickActionEntry* entry = GetQuickActionEntry(actionId);
	if (!entry) {
		spdlog::error("SetSegmentDataFromData: No quick action found for ID {}", actionId);
		return false;
	}
	return entry->action->SetSegmentDataFromData(segment);
}

ModMenuModule::QuickActionId ModMenuModule::QuickActionManager::Add(KeyBindingModule::Key key, QuickActionTypeIndex typeIndex)
{
	spdlog::debug("Adding quick action of type {}", std::type_index(typeIndex).name());

	auto actionItem = QuickActionRegistry::GetByTypeIndex(typeIndex);
	if (actionItem == nullptr) {
		spdlog::error("Add: No quick action type found for index {}", std::type_index(typeIndex).name());
		return 0;
	}

	QuickActionId id = m_nextQuickActionId++;
	AddQuickActionData data;
	data.actionId = id;
	data.key = key;
	data.typeIndex = typeIndex;
	data.actionItem = actionItem;
	AddQuickActionInternal(data);

	return id;
}

void ModMenuModule::QuickActionManager::Update(QuickActionId actionId, const QuickActionInfo& info)
{
	spdlog::debug("Updating quick action ID {}", actionId);
	QuickActionEntry* entry = GetQuickActionEntry(actionId);
	if (!entry) {
		spdlog::error("Update: No quick action found for ID {}", actionId);
		return;
	}
	KeyBindingModule::BindManager* bindManager = KeyBindingModule::BindManager::GetInstance();
	std::string keyBindName = GetKeyBindName(actionId);
	bindManager->SetBind(keyBindName, info.key);
	entry->customLabel = info.customLabel;
}

void ModMenuModule::QuickActionManager::Remove(QuickActionId actionId)
{
	spdlog::debug("Removing quick action ID {}", actionId);

	QuickActionEntry* entry = GetQuickActionEntry(actionId);
	if (!entry) {
		spdlog::error("Remove: No quick action found for ID {}", actionId);
		return;
	}
	KeyBindingModule::BindManager* bindManager = KeyBindingModule::BindManager::GetInstance();
	std::string keyBindName = GetKeyBindName(actionId);
	bindManager->RemoveBind(keyBindName);
	m_quickActions.erase(actionId);
}

ModMenuModule::QuickActionManager::QuickActionManager() 
{
	assert(!m_instance && "QuickActionManager instance already exists");
	m_instance = this;
}

ModMenuModule::QuickActionManager::~QuickActionManager() 
{
	m_instance = nullptr;
}

std::string ModMenuModule::QuickActionManager::GetKeyBindName(QuickActionId actionId)
{
	return "ModMenu_QuickAction_" + std::to_string(actionId);
}

void ModMenuModule::QuickActionManager::Attach() 
{
	LoadFromPersistence();
	Utils::AddDefaultActions(m_loadedDefaultsVersion);
	m_loadedDefaultsVersion = CURRENT_QUICK_ACTION_DEFAULTS_VERSION;

	AddEventListener<KeyDownEvent>(&QuickActionManager::OnKeyDown);
}

void ModMenuModule::QuickActionManager::Detach() 
{
	RemoveEventListener<KeyDownEvent>();
	SaveToPersistence();
}

void ModMenuModule::QuickActionManager::OnKeyDown(KeyDownEvent& event) 
{
	KeyBindingModule::Key pressedKey = KeyBindingModule::Key::FromKeyDownEvent(event);
	for (auto& pair : m_quickActions) {
		QuickActionEntry& entry = pair.second;
		if (!entry.keyBind || *entry.keyBind != pressedKey) continue;
		if (!entry.action) continue;

		spdlog::debug("Executing quick action ID {}", pair.first);
		entry.action->Execute();
	}
}

ModMenuModule::QuickActionManager::QuickActionEntry* ModMenuModule::QuickActionManager::GetQuickActionEntry(QuickActionId actionId)
{
	auto it = m_quickActions.find(actionId);
	if (it == m_quickActions.end()) {
		return nullptr;
	}
	return &(it->second);
}

void ModMenuModule::QuickActionManager::AddQuickActionInternal(const AddQuickActionData& data)
{
	QuickActionEntry newEntry;

	KeyBindingModule::BindManager* bindManager = KeyBindingModule::BindManager::GetInstance();
	bool hasKey = data.key.keyCode != Game::KeyCode::DIK_NONE;
	std::string keyBindName = GetKeyBindName(data.actionId);
	newEntry.keyBind = hasKey ? bindManager->SetBind(keyBindName, data.key) : bindManager->GetBind(keyBindName);
	newEntry.action = std::unique_ptr<QuickActionBase>(data.actionItem->factory());
	newEntry.typeIndex = data.typeIndex;
	newEntry.customLabel = data.customLabel;

	if (!data.serializedData.empty()) {
		newEntry.action->DeserializeData(data.serializedData);
	}

	m_quickActions[data.actionId] = std::move(newEntry);
}

void ModMenuModule::QuickActionManager::SaveToPersistence() const
{
	spdlog::debug("Saving quick actions to persistence");
	PersistenceModule::PersistenceManager* persistenceManager = PersistenceModule::PersistenceManager::GetInstance();

	std::vector<std::vector<uint8_t>> serializedDataCache = {};

	// compute required buffer size
	size_t totalSize = 0;
	totalSize += sizeof(QuickActionId);
	totalSize += sizeof(size_t);
	totalSize += sizeof(size_t);

	for (const auto& pair : m_quickActions) {
		const QuickActionEntry& entry = pair.second;
		const QuickActionRegistryItem* actionItem = QuickActionRegistry::GetByTypeIndex(entry.typeIndex);
		size_t typeIdLength = actionItem->typeId.size();
		size_t labelLength = entry.customLabel.size();

		totalSize += sizeof(QuickActionId);
		totalSize += 4;
		totalSize += typeIdLength;
		totalSize += sizeof(size_t);
		totalSize += labelLength * sizeof(wchar_t);

		std::vector<uint8_t> serializedData = entry.action->SerializeData();
		serializedDataCache.push_back(std::move(serializedData));
		size_t dataLength = serializedDataCache.back().size();
		totalSize += sizeof(size_t);
		totalSize += dataLength;
	}

	std::vector<uint8_t> buffer;
	buffer.resize(totalSize);
	size_t offset = 0;

	auto write = [&buffer, &offset](const void* src, size_t len) {
		if (len == 0) return;
		std::memcpy(buffer.data() + offset, src, len);
		offset += len;
	};

	// next action ID
	write(&m_nextQuickActionId, sizeof(QuickActionId));

	uint16_t version = CURRENT_QUICK_ACTIONS_VERSION;
	write(&version, sizeof(uint16_t));

	uint16_t defaultsVersion = CURRENT_QUICK_ACTION_DEFAULTS_VERSION;
	write(&defaultsVersion, sizeof(uint16_t));

	// number of quick actions
	size_t actionCount = m_quickActions.size();
	write(&actionCount, sizeof(size_t));

	size_t serializedDataIndex = 0;
	for (const auto& pair : m_quickActions) {
		QuickActionId actionId = pair.first;
		const QuickActionEntry& entry = pair.second;

		// action ID
		write(&actionId, sizeof(QuickActionId));

		// action type id length and data
		const QuickActionRegistryItem* actionItem = QuickActionRegistry::GetByTypeIndex(entry.typeIndex);
		size_t typeIdLength = actionItem->typeId.size();
		write(&typeIdLength, sizeof(size_t));
		if (typeIdLength > 0) {
			write(actionItem->typeId.data(), typeIdLength);
		}

		// custom label length and data (wchar_t)
		size_t labelLength = entry.customLabel.size();
		write(&labelLength, sizeof(size_t));
		if (labelLength > 0) {
			write(entry.customLabel.data(), labelLength * sizeof(wchar_t));
		}

		// serialized data length and data
		const std::vector<uint8_t>& serializedData = serializedDataCache[serializedDataIndex++];
		size_t dataLength = serializedData.size();
		write(&dataLength, sizeof(size_t));
		if (dataLength > 0) {
			write(serializedData.data(), dataLength);
		}
	}

	persistenceManager->SaveRaw("ModMenu_QuickActionManager_Actions", buffer.data(), offset);
}

void ModMenuModule::QuickActionManager::LoadFromPersistence()
{
	spdlog::debug("Loading quick actions from persistence");
	PersistenceModule::PersistenceManager* persistenceManager = PersistenceModule::PersistenceManager::GetInstance();

	std::unique_ptr<uint8_t[]> dataPtr = nullptr;
	size_t size = 0;
	if (!persistenceManager->LoadRaw("ModMenu_QuickActionManager_Actions", dataPtr, size)) {
		spdlog::warn("No saved quick action data found");
		return;
	}
	uint8_t* data = dataPtr.get();

	size_t offset = 0;
	auto readUint16 = [&data, &offset, size]() -> std::optional<uint16_t> {
		if (offset + sizeof(uint16_t) > size) return std::nullopt;
		uint16_t value;
		std::memcpy(&value, data + offset, sizeof(uint16_t));
		offset += sizeof(uint16_t);
		return value;
	};
	auto readSizeT = [&data, &offset, size]() -> std::optional<size_t> {
		if (offset + sizeof(size_t) > size) return std::nullopt;
		size_t value;
		std::memcpy(&value, data + offset, sizeof(size_t));
		offset += sizeof(size_t);
		return value;
	};
	auto readQuickActionId = [&data, &offset, size]() -> std::optional<QuickActionId> {
		if (offset + sizeof(QuickActionId) > size) return std::nullopt;
		QuickActionId value;
		std::memcpy(&value, data + offset, sizeof(QuickActionId));
		offset += sizeof(QuickActionId);
		return value;
	};
	auto readString = [&data, &offset, size](size_t length) -> std::optional<std::string> {
		if (length == 0) return std::optional<std::string>(std::string());
		if (offset + length > size) return std::nullopt;
		std::string value(reinterpret_cast<char*>(data + offset), length);
		offset += length;
		return value;
	};
	auto readWString = [&data, &offset, size](size_t length) -> std::optional<std::wstring> {
		if (length == 0) return std::optional<std::wstring>(std::wstring());
		if (offset + length * sizeof(wchar_t) > size) return std::nullopt;
		std::wstring value(reinterpret_cast<wchar_t*>(data + offset), length);
		offset += length * sizeof(wchar_t);
		return value;
	};

	// next quick action ID
	auto nextIdOpt = readSizeT();
	if (!nextIdOpt.has_value()) {
		spdlog::error("Failed to read next quick action ID from persistence data");
		return;
	}

	auto versionOpt = readUint16();
	if (!versionOpt.has_value()) {
		spdlog::error("Failed to read version from persistence data");
		return;
	}

	if (versionOpt.value() != 1) {
		spdlog::warn("Unsupported quick action data version: {}", versionOpt.value());
		return;
	}

	auto defaultsVersionOpt = readUint16();
	if (!defaultsVersionOpt.has_value()) {
		spdlog::error("Failed to read defaults version from persistence data");
		return;
	}
	m_loadedDefaultsVersion = defaultsVersionOpt.value();

	// number of quick actions
	auto actionCountOpt = readSizeT();
	if (!actionCountOpt.has_value()) {
		spdlog::error("Failed to read quick action count from persistence data");
		return;
	}

	std::vector<AddQuickActionData> loadedActions;
	loadedActions.reserve(actionCountOpt.value());
	bool readError = false;

	for (size_t i = 0; i < actionCountOpt.value(); ++i) {
		AddQuickActionData newAction;
		bool skipAction = false;

		// action ID
		auto actionIdOpt = readQuickActionId();
		if (!actionIdOpt.has_value()) {
			readError = true;
			break;
		}
		newAction.actionId = actionIdOpt.value();

		// action type id
		auto typeIdLengthOpt = readSizeT();
		if (!typeIdLengthOpt.has_value()) {
			readError = true;
			break;
		}
		auto typeIdOpt = readString(typeIdLengthOpt.value());
		if (!typeIdOpt.has_value()) {
			readError = true;
			break;
		}
		auto& typeId = typeIdOpt.value();
		auto actionItemPair = QuickActionRegistry::GetByTypeId(typeId);
		newAction.typeIndex = actionItemPair.first;
		newAction.actionItem = actionItemPair.second;
		if (actionItemPair.second == nullptr) {
			spdlog::error("No quick action type found for name '{}', skipping", typeId);
			skipAction = true;
		}

		// custom label
		auto labelLengthOpt = readSizeT();
		if (!labelLengthOpt.has_value()) {
			readError = true;
			break;
		}
		if (labelLengthOpt.value() > 0) {
			auto labelOpt = readWString(labelLengthOpt.value());
			if (!labelOpt.has_value()) {
				readError = true;
				break;
			}
			newAction.customLabel = labelOpt.value();
		}

		// serialized data
		auto dataLengthOpt = readSizeT();
		if (!dataLengthOpt.has_value()) {
			readError = true;
			break;
		}
		if (dataLengthOpt.value() > 0) {
			auto dataOpt = readString(dataLengthOpt.value());
			if (!dataOpt.has_value()) {
				readError = true;
				break;
			}
			newAction.serializedData = std::vector<uint8_t>(
				dataOpt.value().begin(),
				dataOpt.value().end()
			);
		}

		if (!skipAction) loadedActions.push_back(newAction);
	}

	if (readError) {
		spdlog::error("Error reading quick action data from persistence");
		return;
	}

	m_nextQuickActionId = static_cast<QuickActionId>(nextIdOpt.value());
	for (const auto& actionData : loadedActions) {
		AddQuickActionInternal(actionData);
	}
}
