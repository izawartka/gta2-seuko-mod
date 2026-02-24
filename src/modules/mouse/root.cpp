#include "root.h"

MouseModule::RootModule* MouseModule::RootModule::m_instance = nullptr;

MouseModule::RootModule::RootModule() {

	assert(m_instance == nullptr && "MouseModule::RootModule instance already exists!");
	m_instance = this;
	spdlog::info("MouseModule::RootModule module initialized.");
}

MouseModule::RootModule::~RootModule()
{
	m_instance = nullptr;
}

MouseModule::RootModule* MouseModule::RootModule::GetInstance()
{
	return m_instance;
}

bool MouseModule::RootModule::Attach()
{
	m_mouseManager.Attach();
	spdlog::info("MouseModule::RootModule module attached.");
	return true;
}

void MouseModule::RootModule::Detach()
{
	m_mouseManager.Detach();
	spdlog::info("MouseModule::RootModule module detached.");
}
