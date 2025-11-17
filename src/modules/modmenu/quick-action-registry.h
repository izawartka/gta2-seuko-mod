#pragma once
#include "quick-action-base.h"

namespace ModMenuModule {
	using QuickActionTypeIndex = std::type_index;
	using QuickActionFactory = std::function<QuickActionBase* ()>;
	
	struct QuickActionRegistryItem {
		QuickActionFactory factory;
		const std::string& typeId;
		const std::wstring& typeLabel;
	};

	class QuickActionRegistry {
	private:
		friend class QuickActionManager;

		static std::unordered_map<QuickActionTypeIndex, QuickActionRegistryItem>& Actions() {
			static std::unordered_map<QuickActionTypeIndex, QuickActionRegistryItem> actions;
			return actions;
		}

	public:
		static std::pair<QuickActionTypeIndex, const QuickActionRegistryItem*> GetByTypeId(const std::string& typeId) {
			auto it = std::find_if(
				Actions().begin(),
				Actions().end(),
				[&typeId](const auto& pair) {
					return pair.second.typeId == typeId;
				}
			);
			if (it != Actions().end()) {
				return { it->first, &(it->second) };
			}
			return { std::type_index(typeid(void)), nullptr };
		}

		static const QuickActionRegistryItem* GetByTypeIndex(QuickActionTypeIndex typeIndex) {
			auto& actions = Actions();
			auto it = actions.find(typeIndex);
			if (it == actions.end()) {
				return nullptr;
			}
			return &(it->second);
		}

		template<typename T>
		static void Register(QuickActionRegistryItem quickAction) {
			auto& actions = Actions();
			QuickActionTypeIndex type = QuickActionTypeIndex(typeid(T));
			if (actions.find(type) != actions.end()) return;
			actions.insert({ type, quickAction });
		}
	};
}

#define REGISTER_QUICK_ACTION(CLASSNAME) \
namespace { \
	struct CLASSNAME##_QuickActionReg { \
		CLASSNAME##_QuickActionReg() { \
			ModMenuModule::QuickActionRegistry::Register<ModMenuModule::CLASSNAME>({ \
				[]() { return new ModMenuModule::CLASSNAME(); }, \
				ModMenuModule::CLASSNAME::GetTypeId(), \
				ModMenuModule::CLASSNAME::GetTypeLabel() \
			}); \
		} \
	}; \
	static CLASSNAME##_QuickActionReg s_##CLASSNAME##_QuickActionReg; \
}
