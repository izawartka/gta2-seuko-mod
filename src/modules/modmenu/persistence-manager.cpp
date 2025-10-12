#include "persistence-manager.h"

ModMenuModule::PersistenceManager* ModMenuModule::PersistenceManager::m_instance = nullptr;

ModMenuModule::PersistenceManager::PersistenceManager() {
	assert(!m_instance, "PersistenceManager instance already exists");
	m_instance = this;
}

ModMenuModule::PersistenceManager::~PersistenceManager() {
	m_instance = nullptr;
}

ModMenuModule::PersistenceManager* ModMenuModule::PersistenceManager::GetInstance() {
	assert(m_instance != nullptr, "PersistenceManager not initialized!");
	return m_instance;
}
