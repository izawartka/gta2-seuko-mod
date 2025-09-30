#include "common.h"
#include "module-manager.h"

Core::ModuleManager* Core::ModuleManager::m_instance = nullptr;

Core::ModuleManager::ModuleManager()
{
	assert(m_instance == nullptr, "ModuleManager instance already exists!");
	m_instance = this;
}

Core::ModuleManager::~ModuleManager()
{
	RemoveAllModules();
	m_instance = nullptr;
}

Core::ModuleManager* Core::ModuleManager::GetInstance()
{
	assert(m_instance != nullptr, "Core not initialized!");
	return m_instance;
}
