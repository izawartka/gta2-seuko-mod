#include "common.h"
#include "event-manager.h"

Core::EventManager* Core::EventManager::m_instance = nullptr;

Core::EventManager::EventManager()
{
	assert(m_instance == nullptr && "EventManager instance already exists!");
	m_instance = this;
}

Core::EventManager::~EventManager()
{
	m_instance = nullptr;
}

Core::EventManager* Core::EventManager::GetInstance()
{
	assert(m_instance != nullptr && "Core not initialized!");
	return m_instance;
}
