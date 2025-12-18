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

		std::unordered_map<std::type_index, std::unique_ptr<CheatBase>> m_cheats;
	};
}
