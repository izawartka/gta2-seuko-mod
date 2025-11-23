#pragma once
#include "common.h"
#include "module-base.h"

namespace Core
{
	class ModuleManager
	{
	public:
		static ModuleManager* GetInstance();

		template<typename ModuleT, typename... Args>
		ModuleT* AddModule(Args&&... args) {
			static_assert(std::is_base_of<ModuleBase, ModuleT>::value, "ModuleT must derive from Core::ModuleBase");
			spdlog::debug("Adding module: {}", typeid(ModuleT).name());

			std::type_index typeIdx(typeid(ModuleT));
			if (m_modules.find(typeIdx) != m_modules.end()) {
				spdlog::warn("Module already added: {}", typeid(ModuleT).name());
				return static_cast<ModuleT*>(m_modules[typeIdx].get());
			}

			auto module = std::make_unique<ModuleT>(std::forward<Args>(args)...);
			if (!module->Attach()) {
				spdlog::error("Failed to attach module: {}", typeid(ModuleT).name());
				return nullptr;
			}

			ModuleT* modulePtr = module.get();
			m_modules[typeIdx] = std::move(module);
			return modulePtr;
		}

		template<typename ModuleT>
		void RemoveModule() {
			static_assert(std::is_base_of<ModuleBase, ModuleT>::value, "ModuleT must derive from Core::ModuleBase");
			spdlog::debug("Removing module: {}", typeid(ModuleT).name());

			std::type_index typeIdx(typeid(ModuleT));
			auto it = m_modules.find(typeIdx);
			if(it == m_modules.end()) {
				spdlog::warn("Module not found: {}", typeid(ModuleT).name());
				return;
			}

			it->second->Detach();
			m_modules.erase(it);
		}

		template<typename ModuleT>
		ModuleT* GetModule() {
			static_assert(std::is_base_of<ModuleBase, ModuleT>::value, "ModuleT must derive from Core::ModuleBase");
			std::type_index typeIdx(typeid(ModuleT));
			auto it = m_modules.find(typeIdx);
			if(it == m_modules.end()) {
				spdlog::warn("Module not found: {}", typeid(ModuleT).name());
				return nullptr;
			}
			return static_cast<ModuleT*>(it->second.get());
		}

		void RemoveAllModules() {
			while (!m_modules.empty()) {
				auto it = std::prev(m_modules.end());
				it->second->Detach();
				m_modules.erase(it);
			}
		}

	private:
		friend class Core;
		ModuleManager();
		~ModuleManager();
		ModuleManager(const ModuleManager&) = delete;
		ModuleManager& operator=(const ModuleManager&) = delete;

		std::unordered_map<std::type_index, std::unique_ptr<ModuleBase>> m_modules;
		static ModuleManager* m_instance;
	};
}
