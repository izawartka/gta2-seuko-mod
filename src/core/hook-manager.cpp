#include "hook-manager.h"

Core::HookManager* Core::HookManager::m_instance = nullptr;

Core::HookManager::HookManager()
{
	assert(m_instance == nullptr && "HookManager instance already exists!");
	m_instance = this;
}

Core::HookManager::~HookManager()
{
	m_instance = nullptr;
}

Core::HookManager* Core::HookManager::GetInstance()
{
	assert(m_instance != nullptr && "Core not initialized!");
	return m_instance;
}
