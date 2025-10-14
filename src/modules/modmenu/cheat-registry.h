#pragma once
#include "cheat-base.h"

namespace ModMenuModule {
	class CheatRegistry {
	public:
		using Factory = std::function<CheatBase* ()>;
		static std::vector<Factory>& Factories() {
			static std::vector<Factory> factories;
			return factories;
		}
		static void Register(Factory factory) {
			Factories().push_back(factory);
		}
	};
}

// Macro for registering a cheat
#define REGISTER_CHEAT(CLASSNAME) \
namespace { \
	struct CLASSNAME##_CheatReg { \
		CLASSNAME##_CheatReg() { \
			ModMenuModule::CheatRegistry::Register([]() { return new ModMenuModule::##CLASSNAME##(); }); \
		} \
	}; \
	static CLASSNAME##_CheatReg s_##CLASSNAME##_CheatReg; \
}
