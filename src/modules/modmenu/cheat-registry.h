#pragma once
#include "common.h"

namespace ModMenuModule {
	class CheatBase;
	using CheatTypeIndex = std::type_index;
	using CheatFactory = std::function<CheatBase* ()>;
}

namespace {
	template<typename CheatT>
	ModMenuModule::CheatBase* CheatFactoryFunc() {
		return new CheatT();
	}

	template<typename CheatT, typename = void>
	struct HasGetDependencies : std::false_type {};

	template<typename CheatT>
	struct HasGetDependencies<CheatT, std::void_t<decltype(CheatT::GetDependencies())>> : std::true_type {};

	template<typename CheatT>
	std::vector<ModMenuModule::CheatTypeIndex> GetCheatDependencies() {
		if constexpr (HasGetDependencies<CheatT>::value) {
			auto deps = CheatT::GetDependencies();
			std::vector<ModMenuModule::CheatTypeIndex> result;
			result.reserve(deps.size());
			for (const auto& dep : deps) {
				result.push_back(dep);
			}
			return result;
		}
		else {
			return {};
		}
	}
}

namespace ModMenuModule {
	class CheatRegistry {
	private:
		friend class CheatManager;

		struct RegistryItem {
			CheatFactory factory;
			std::vector<CheatTypeIndex> dependencies;

			RegistryItem() = default;

			RegistryItem(CheatFactory factory, std::vector<CheatTypeIndex> dependencies)
				: factory(std::move(factory))
				, dependencies(std::move(dependencies))
			{
			}
		};

		static std::unordered_map<CheatTypeIndex, RegistryItem>& Cheats() {
			static std::unordered_map<CheatTypeIndex, RegistryItem> cheats;
			return cheats;
		}

	public:
		template<typename CheatT>
		static void Register() {
			static_assert(std::is_base_of<CheatBase, CheatT>::value, "CheatT must derive from CheatBase");

			auto& cheats = Cheats();
			CheatTypeIndex type = CheatTypeIndex(typeid(CheatT));
			if (cheats.find(type) != cheats.end()) return;

			RegistryItem item(
				CheatFactoryFunc<CheatT>,
				GetCheatDependencies<CheatT>()
			);

			cheats.insert({ type, item });
		}
	};
}

#define REGISTER_CHEAT(CLASSNAME) \
namespace { \
	struct CLASSNAME##_CheatReg { \
		CLASSNAME##_CheatReg() { \
			ModMenuModule::CheatRegistry::Register<ModMenuModule::CLASSNAME>(); \
		} \
	}; \
	static CLASSNAME##_CheatReg s_##CLASSNAME##_CheatReg; \
}
