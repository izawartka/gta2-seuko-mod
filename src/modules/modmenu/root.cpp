#include "root.h"
#include "cheat-registry.h"

ModMenuModule::RootModule* ModMenuModule::RootModule::m_instance = nullptr;

ModMenuModule::RootModule::RootModule(ModMenuOptions options) : m_options(options) {

	assert(UiModule::RootModule::GetInstance() != nullptr, "ModMenu::RootModule requires Ui::RootModule to be initialized first!");
	assert(m_instance == nullptr, "ModMenu::RootModule instance already exists!");
	m_instance = this;
	spdlog::info("ModMenu::RootModule module initialized.");
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
	spdlog::info("ModMenu::RootModule module attached.");

	InstantiateCheats();
	m_menuManager.Attach();
	return true;
}

void ModMenuModule::RootModule::Detach()
{
	m_menuManager.Detach();
	spdlog::info("ModMenu::RootModule module detached.");
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
