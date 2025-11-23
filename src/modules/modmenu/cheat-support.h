#pragma once
#include "common.h"
#include "root.h"

namespace ModMenuModule {

	class CheatSupport {
	protected:
		CheatSupport() = default;
		~CheatSupport() = default;

		template<typename CheatT>
		CheatT* GetCheat() {
			static_assert(std::is_base_of<CheatBase, CheatT>::value, "CheatT must derive from ModMenuModule::CheatBase");
			auto* root = RootModule::GetInstance();
			return root->GetCheat<CheatT>();
		}

		template<typename CheatT>
		void SetCheatEnabled(bool enabled) {
			static_assert(std::is_base_of<CheatBase, CheatT>::value, "CheatT must derive from ModMenuModule::CheatBase");
			auto* root = RootModule::GetInstance();
			CheatT* cheat = root->GetCheat<CheatT>();
			if (cheat) {
				cheat->SetEnabled(enabled);
			}
		}

		template<typename CheatT>
		bool IsCheatEnabled() {
			static_assert(std::is_base_of<CheatBase, CheatT>::value, "CheatT must derive from ModMenuModule::CheatBase");
			auto* root = RootModule::GetInstance();
			CheatT* cheat = root->GetCheat<CheatT>();
			if (cheat) {
				return cheat->IsEnabled();
			}
			
			return false;
		}
	};
}
