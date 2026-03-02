#pragma once
#include "common.h"
#include "cheat-base.h"

namespace ModMenuModule {
	class CheatManager {
	private:
		friend class RootModule;
		CheatManager();
		virtual ~CheatManager();
		CheatManager(const CheatManager&) = delete;
		CheatManager& operator=(const CheatManager&) = delete;

		void Attach();
		void Detach();

		static CheatManager* m_instance;

		void InstantiateCheats();
		void DestroyCheats();
		
		static std::vector<std::type_index> TopologicalSort();

		std::unordered_map<std::type_index, std::unique_ptr<CheatBase>> m_cheats;
		std::vector<std::type_index> m_attachOrder;
	};
}
