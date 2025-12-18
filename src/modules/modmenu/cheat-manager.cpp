#include "cheat-manager.h"
#include "cheat-registry.h"

ModMenuModule::CheatManager* ModMenuModule::CheatManager::m_instance = nullptr;

ModMenuModule::CheatManager::CheatManager()
{
	assert(!m_instance && "CheatManager instance already exists");
	m_instance = this;
}

ModMenuModule::CheatManager::~CheatManager()
{
	m_instance = nullptr;
}

void ModMenuModule::CheatManager::Attach()
{
	InstantiateCheats();
}

void ModMenuModule::CheatManager::Detach()
{
	DestroyCheats();
}

void ModMenuModule::CheatManager::InstantiateCheats()
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

void ModMenuModule::CheatManager::DestroyCheats()
{
	while (!m_cheats.empty()) {
		auto it = m_cheats.begin();
		ModMenuModule::CheatBase* cheat = it->second.get();
		cheat->Detach();
		m_cheats.erase(it);
	}
}
