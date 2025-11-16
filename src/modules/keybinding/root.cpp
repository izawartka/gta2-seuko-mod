#include "root.h"

KeyBindingModule::RootModule* KeyBindingModule::RootModule::m_instance = nullptr;

KeyBindingModule::RootModule::RootModule() {

	assert(PersistenceModule::RootModule::GetInstance() != nullptr, "KeyBindingModule::RootModule requires PersistenceModule::RootModule to be initialized first!");
	assert(m_instance == nullptr, "KeyBindingModule::RootModule instance already exists!");
	m_instance = this;
	spdlog::info("KeyBindingModule::RootModule module initialized.");
}

KeyBindingModule::RootModule::~RootModule()
{
	m_instance = nullptr;
}

KeyBindingModule::RootModule* KeyBindingModule::RootModule::GetInstance()
{
	return m_instance;
}

bool KeyBindingModule::RootModule::Attach()
{
	m_bindManager.LoadFromPersistence();
	spdlog::info("KeyBindingModule::RootModule module attached.");

	return true;
}

void KeyBindingModule::RootModule::Detach()
{
	m_bindManager.SaveToPersistence();
	spdlog::info("KeyBindingModule::RootModule module detached.");
}
