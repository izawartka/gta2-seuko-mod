#include "root.h"
#include "cheat-registry.h"

ModMenuModule::RootModule* ModMenuModule::RootModule::m_instance = nullptr;

ModMenuModule::RootModule::RootModule(const ModMenuOptions& options) : m_options(options) {
	assert(PersistenceModule::RootModule::GetInstance() != nullptr && "ModMenuModule::RootModule requires PersistenceModule::RootModule to be initialized first!");
	assert(KeyBindingModule::RootModule::GetInstance() != nullptr && "ModMenuModule::RootModule requires KeyBindingModule::RootModule to be initialized first!");
	assert(UiModule::RootModule::GetInstance() != nullptr && "ModMenuModule::RootModule requires Ui::RootModule to be initialized first!");
	assert(m_instance == nullptr && "ModMenuModule::RootModule instance already exists!");
	m_instance = this;
	spdlog::info("ModMenuModule::RootModule module initialized.");
}

ModMenuModule::RootModule::~RootModule()
{
	m_instance = nullptr;
}

ModMenuModule::RootModule* ModMenuModule::RootModule::GetInstance()
{
	return m_instance;
}

bool ModMenuModule::RootModule::Attach()
{
	m_toastManager.Attach();
	m_cheatManager.Attach();
	m_menuManager.Attach();
	m_resetBindsManager.Attach();
	m_quickActionManager.Attach();
	spdlog::info("ModMenuModule::RootModule module attached.");

	return true;
}

void ModMenuModule::RootModule::Detach()
{
	m_quickActionManager.Detach();
	m_resetBindsManager.Detach();
	m_menuManager.Detach();
	m_cheatManager.Detach();
	m_toastManager.Detach();
	spdlog::info("ModMenuModule::RootModule module detached.");
}

