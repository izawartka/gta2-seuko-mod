#pragma once
#include "common.h"
#include "hook-base.h"

namespace Core
{
	class HookManager
	{
	public:
		static HookManager* GetInstance();

		template<typename HookT, typename... Args>
		bool AddHook(const HookT& hookDef, Args... args) {
			static_assert(std::is_base_of<HookBase, HookT>::value, "HookT must derive from Core::HookBase");
			spdlog::debug("Adding hook: {}", typeid(HookT).name());
			
			if (m_hooks.find(&hookDef) != m_hooks.end()) {
				spdlog::debug("Hook already added: {}", typeid(HookT).name());
				return true;
			}

			if (!HookT::Hook(hookDef, std::forward<Args>(args)...)) {
				spdlog::error("Failed to add hook: {}", typeid(HookT).name());
				return false;
			}

			m_hooks.insert(&hookDef);
			return true;
		}

	private:
		friend class Core;
		HookManager();
		~HookManager();
		HookManager(const HookManager&) = delete;
		HookManager& operator=(const HookManager&) = delete;

		std::unordered_set<const HookBase*> m_hooks;
		static HookManager* m_instance;
	};
}
