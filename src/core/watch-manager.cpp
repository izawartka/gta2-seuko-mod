#include "common.h"
#include "watch-manager.h"

Core::WatchManager* Core::WatchManager::m_instance = nullptr;

Core::WatchManager* Core::WatchManager::GetInstance()
{
	assert(m_instance != nullptr && "WatchManager not initialized!");
	return m_instance;
}

void Core::WatchManager::Unwatch(WatchedBase* watched)
{
	if (watched == nullptr) return;

	watched->m_willDestroy = true;

	if (m_updating) {
		PendingChange change(ChangeType::Remove, watched->GetId(), typeid(void), nullptr);
		m_pendingChanges.push(change);
	}
	else {
		RemoveEntry(watched->GetId());
	}
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

void Core::WatchManager::AddEntry(WatchedBase* entry, std::type_index eventType)
{
	spdlog::debug("Adding watched: {} with id: {}", typeid(*entry).name(), entry->GetId());

	m_entries[eventType].push_back(std::unique_ptr<WatchedBase>(entry));
	m_idToEventType.insert({ entry->GetId(), eventType});
}

void Core::WatchManager::RemoveEntry(WatchedId id)
{
	spdlog::debug("Removing watched with id: {}", id);

	auto it = m_idToEventType.find(id);
	if (it == m_idToEventType.end()) {
		spdlog::warn("No watched found with id: {}", id);
		return;
	}

	std::type_index eventTypeIdx = it->second;
	m_idToEventType.erase(it);

	auto& vec = m_entries.at(eventTypeIdx);
	vec.erase(std::remove_if(vec.begin(), vec.end(),
		[id](const auto& entry) { return entry->m_id == id; }), vec.end());
}

void Core::WatchManager::RemoveEventType(std::type_index eventType)
{
	spdlog::debug("No more watched for event type: {}, removing listener", eventType.name());

	auto entriesIt = m_entries.find(eventType);
	if (entriesIt == m_entries.end()) {
		spdlog::error("Could not find entries vector for event type: {}", eventType.name());
	}
	else {
		m_entries.erase(entriesIt);
	}

	auto it = m_typeToListenerId.find(eventType);
	if (it == m_typeToListenerId.end()) {
		spdlog::error("Could not find listener ID for event type: {}", eventType.name());
		return;
	}

	EventManager::GetInstance()->RemoveListener(eventType, it->second);
	m_typeToListenerId.erase(it);
}

void Core::WatchManager::ProcessPendingChanges()
{
	while (!m_pendingChanges.empty()) {
		PendingChange change = m_pendingChanges.front();
		m_pendingChanges.pop();

		if (change.type == ChangeType::Add) {
			AddEntry(change.entry, change.eventType);
		}
		else if (change.type == ChangeType::Remove) {
			RemoveEntry(change.id);
		}
	}
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
		if (entry->GetWillDestroy()) continue;
		entry->Update();
	}

	m_updating = false;
	ProcessPendingChanges();
}
