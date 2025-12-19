#include "common.h"
#include "watch-manager.h"

Core::WatchManager* Core::WatchManager::m_instance = nullptr;

Core::WatchManager* Core::WatchManager::GetInstance()
{
	assert(m_instance != nullptr && "WatchManager not initialized!");
	return m_instance;
}

void Core::WatchManager::Unwatch(WatchedId id)
{
	spdlog::debug("Removing watched with id: {}", id);

	auto it = std::find_if(m_idToEventType.begin(), m_idToEventType.end(),
		[id](const auto& pair) { return pair.first == id; });

	if (it == m_idToEventType.end()) {
		spdlog::warn("No watched found with id: {}", id);
		return;
	}

	auto eventTypeIdx = it->second;

	if (m_updating) {
		PendingChange change(ChangeType::Remove, id, eventTypeIdx, nullptr);
		m_pendingChanges.push(change);
	}
	else {
		RemoveEntry(id, eventTypeIdx);
	}
}

void Core::WatchManager::Unwatch(WatchedBase* watched)
{
	if (watched == nullptr) return;
	Unwatch(watched->GetId());
}

Core::WatchManager::WatchManager()
{
	assert(m_instance == nullptr && "WatchManager instance already exists!");
	m_instance = this;
}

Core::WatchManager::~WatchManager()
{
	spdlog::debug("WatchManager::~WatchManager: Removing all event listeners");
	for (auto& [eventType, listenerId] : m_typeToListenerId) {
		EventManager::GetInstance()->RemoveListener(eventType, listenerId);
	}

	m_instance = nullptr;
}

void Core::WatchManager::RemoveEntry(WatchedId id, std::type_index eventTypeIdx)
{
	auto idIt = std::find_if(m_idToEventType.begin(), m_idToEventType.end(),
		[id](const auto& pair) { return pair.first == id; });

	if (idIt != m_idToEventType.end()) {
		m_idToEventType.erase(idIt);
	}

	auto& vec = m_entries[eventTypeIdx];
	vec.erase(std::remove_if(vec.begin(), vec.end(),
		[id](const auto& entry) { return entry->m_id == id; }), vec.end());
}

void Core::WatchManager::RemoveEventType(std::type_index eventType)
{
	// Note: m_entries entry is not being removed
	spdlog::debug("No more watched for event type: {}, removing listener", eventType.name());

	auto it = m_typeToListenerId.find(eventType);
	if (it == m_typeToListenerId.end()) return;
	EventManager::GetInstance()->RemoveListener(eventType, it->second);
	m_typeToListenerId.erase(it);
}

void Core::WatchManager::ProcessPendingChanges()
{
	while (!m_pendingChanges.empty()) {
		PendingChange change = m_pendingChanges.front();
		m_pendingChanges.pop();

		if (change.type == ChangeType::Add) {
			ApplyAdd(change);
		}
		else if (change.type == ChangeType::Remove) {
			ApplyRemove(change);
		}
	}
}

void Core::WatchManager::ApplyAdd(PendingChange& change)
{
	WatchedBase* entry = change.entry;
	WatchedId id = entry->m_id;
	std::type_index eventTypeIdx = change.eventType;

	spdlog::debug("Applying queued add for watched: {} with id: {} (Event: {})",
		typeid(*entry).name(), id, eventTypeIdx.name());

	m_entries[eventTypeIdx].push_back(std::unique_ptr<WatchedBase>(entry));
	m_idToEventType.push_back({ id, eventTypeIdx });
}

void Core::WatchManager::ApplyRemove(PendingChange& change)
{
	spdlog::debug("Applying queued remove for watched with id: {}", change.id);
	RemoveEntry(change.id, change.eventType);
}

void Core::WatchManager::Update(std::type_index eventType)
{
	auto it = m_entries.find(eventType);
	if (it == m_entries.end()) return;

	if (it->second.empty()) {
		RemoveEventType(eventType);
		return;
	}

	m_updating = true;

	for (auto& entry : it->second) {
		entry->Update();
	}

	m_updating = false;
	ProcessPendingChanges();
}
