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
	auto& cheats = ModMenuModule::CheatRegistry::Cheats();
	spdlog::info("Instantiating {} cheats", cheats.size());

	for (const auto& pair : cheats) {
		std::type_index typeIdx = pair.first;
		const auto& registryItem = pair.second;

		ModMenuModule::CheatBase* cheat = registryItem.factory();
		if (!cheat) {
			spdlog::error("Cheat factory returned null pointer");
			continue;
		}

		if (m_cheats.find(typeIdx) != m_cheats.end()) {
			spdlog::error("Cheat {} is already registered", typeIdx.name());
			delete cheat;
			continue;
		}

		m_cheats[typeIdx] = std::unique_ptr<ModMenuModule::CheatBase>(cheat);
	}

	m_attachOrder = TopologicalSort();

	for (std::type_index typeIdx : m_attachOrder) {
		auto it = m_cheats.find(typeIdx);
		if (it == m_cheats.end()) continue;

		ModMenuModule::CheatBase* cheat = it->second.get();
		if (!cheat->Attach()) {
			spdlog::error("Failed to attach cheat {}", typeIdx.name());
		}
	}
}

void ModMenuModule::CheatManager::DestroyCheats()
{
	for (auto it = m_attachOrder.rbegin(); it != m_attachOrder.rend(); ++it) {
		std::type_index typeIdx = *it;
		auto cheatIt = m_cheats.find(typeIdx);
		if (cheatIt == m_cheats.end()) continue;

		ModMenuModule::CheatBase* cheat = cheatIt->second.get();
		cheat->Detach();
	}

	m_cheats.clear();
	m_attachOrder.clear();
}

std::vector<std::type_index> ModMenuModule::CheatManager::TopologicalSort()
{
	auto& registry = ModMenuModule::CheatRegistry::Cheats();
	std::vector<std::type_index> result;
	std::unordered_set<std::type_index> visiting;
	std::unordered_set<std::type_index> visited;

	std::function<void(std::type_index)> visit = [&](std::type_index cheatType) {
		if (visited.find(cheatType) != visited.end()) return;

		if (visiting.find(cheatType) != visiting.end()) {
			spdlog::error("Cyclic dependency detected for cheat {}", cheatType.name());
			return;
		}

		auto it = registry.find(cheatType);
		if (it == registry.end()) {
			spdlog::error("Cheat {} not found in registry", cheatType.name());
			return;
		}

		visiting.insert(cheatType);

		for (const auto& dependency : it->second.dependencies) {
			if (registry.find(dependency) == registry.end()) {
				spdlog::error("Cheat {} has missing dependency {}", cheatType.name(), dependency.name());
				continue;
			}
			visit(dependency);
		}

		visiting.erase(cheatType);
		visited.insert(cheatType);
		result.push_back(cheatType);
	};

	for (const auto& pair : registry) {
		visit(pair.first);
	}

	return result;
}
