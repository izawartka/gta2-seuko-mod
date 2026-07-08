#include "cheat-manager.h"
#include "cheat-registry.h"

ModMenuModule::CheatManager* ModMenuModule::CheatManager::m_instance = nullptr;

ModMenuModule::CheatManager::CheatManager()
{
	assert(!m_instance && "CheatManager instance already exists");
	m_instance = this;

	InstantiateCheats();
}

ModMenuModule::CheatManager::~CheatManager()
{
	DestroyCheats();

	m_instance = nullptr;
}

void ModMenuModule::CheatManager::Attach()
{
	AttachCheats();
}

void ModMenuModule::CheatManager::Detach()
{
	DetachCheats();
}

void ModMenuModule::CheatManager::InstantiateCheats()
{
	auto& cheats = ModMenuModule::CheatRegistry::Cheats();
	spdlog::info("CheatManager: Instantiating {} cheats", cheats.size());

	for (const auto& pair : cheats) {
		std::type_index typeIdx = pair.first;
		const auto& registryItem = pair.second;

		ModMenuModule::CheatBase* cheat = registryItem.factory();
		if (!cheat) {
			spdlog::error("CheatManager: Cheat factory returned null pointer");
			continue;
		}

		if (m_cheats.find(typeIdx) != m_cheats.end()) {
			spdlog::error("CheatManager: Cheat {} is already registered", typeIdx.name());
			delete cheat;
			continue;
		}

		m_cheats[typeIdx] = std::unique_ptr<ModMenuModule::CheatBase>(cheat);
	}
}

void ModMenuModule::CheatManager::AttachCheats()
{
	spdlog::info("CheatManager: Sorting and attaching cheats");
	m_attachOrder = TopologicalSort();

	for (std::type_index typeIdx : m_attachOrder) {
		auto it = m_cheats.find(typeIdx);
		if (it == m_cheats.end()) continue;

		ModMenuModule::CheatBase* cheat = it->second.get();
		if (!cheat->Attach()) {
			spdlog::error("CheatManager: Failed to attach cheat {}", typeIdx.name());
		}
	}
}

void ModMenuModule::CheatManager::DetachCheats()
{
	spdlog::debug("CheatManager: Detaching all cheats");
	for (auto it = m_attachOrder.rbegin(); it != m_attachOrder.rend(); ++it) {
		std::type_index typeIdx = *it;
		auto cheatIt = m_cheats.find(typeIdx);
		if (cheatIt == m_cheats.end()) continue;

		ModMenuModule::CheatBase* cheat = cheatIt->second.get();
		cheat->Detach();
	}

	m_attachOrder.clear();
}

void ModMenuModule::CheatManager::DestroyCheats()
{
	spdlog::debug("CheatManager: Destroying all cheats");
	m_cheats.clear();
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
			spdlog::error("CheatManager: Cyclic dependency detected for cheat {}", cheatType.name());
			return;
		}

		auto it = registry.find(cheatType);
		if (it == registry.end()) {
			spdlog::error("CheatManager: Cheat {} not found in registry", cheatType.name());
			return;
		}

		visiting.insert(cheatType);

		for (const auto& dependency : it->second.dependencies) {
			if (registry.find(dependency) == registry.end()) {
				spdlog::error("CheatManager: Cheat {} has missing dependency {}", cheatType.name(), dependency.name());
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
