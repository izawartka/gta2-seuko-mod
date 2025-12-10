#include "common.h"
#include "watch-manager.h"

Core::WatchManager* Core::WatchManager::m_instance = nullptr;

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

Core::WatchManager* Core::WatchManager::GetInstance()
{
	assert(m_instance != nullptr && "WatchManager not initialized!");
	return m_instance;
}
