#include "root.h"

PersistenceModule::RootModule* PersistenceModule::RootModule::m_instance = nullptr;

PersistenceModule::RootModule::RootModule() {

	assert(m_instance == nullptr, "PersistenceModule::RootModule instance already exists!");
	m_instance = this;
	spdlog::info("PersistenceModule::RootModule module initialized.");
}

PersistenceModule::RootModule::~RootModule()
{
	m_instance = nullptr;
}

PersistenceModule::RootModule* PersistenceModule::RootModule::GetInstance()
{
	return m_instance;
}

bool PersistenceModule::RootModule::Attach()
{
	m_persistenceManager.LoadFromFile();
	spdlog::info("PersistenceModule::RootModule module attached.");

	return true;
}

void PersistenceModule::RootModule::Detach()
{
	m_persistenceManager.SaveToFile();
	spdlog::info("PersistenceModule::RootModule module detached.");
}
