#include "root.h"
#include "cheat-registry.h"

ModMenuModule::RootModule* ModMenuModule::RootModule::m_instance = nullptr;

ModMenuModule::RootModule::RootModule(ModMenuOptions options) : m_options(options) {
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
	InstantiateCheats();
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
	DestroyCheats();
	m_toastManager.Detach();
	spdlog::info("ModMenuModule::RootModule module detached.");
}

void ModMenuModule::RootModule::InstantiateCheats()
{
	auto& factories = ModMenuModule::CheatRegistry::Factories();
	spdlog::info("Instantiating {} cheats", factories.size());
	for (auto& factory : factories) {
		ModMenuModule::CheatBase* cheat = factory();
		if (!cheat) {
			spdlog::error("Cheat factory returned null pointer");
			continue;
		}

		if (!cheat->Attach()) {
			spdlog::error("Failed to attach cheat {}", typeid(*cheat).name());
			delete cheat;
			continue;
		}

		std::type_index typeIdx(typeid(*cheat));
		if (m_cheats.find(typeIdx) != m_cheats.end()) {
			spdlog::error("Cheat {} is already registered", typeid(*cheat).name());
			cheat->Detach();
			delete cheat;
			continue;
		}

		m_cheats[typeIdx] = std::unique_ptr<ModMenuModule::CheatBase>(cheat);
	}
}

void ModMenuModule::RootModule::DestroyCheats()
{
	while (!m_cheats.empty()) {
		auto it = m_cheats.begin();
		ModMenuModule::CheatBase* cheat = it->second.get();
		cheat->Detach();
		m_cheats.erase(it);
	}
}
