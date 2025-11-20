#pragma once
#include "quick-action-base.h"

namespace ModMenuModule {
	class SegmentBase;

	using QuickActionTypeIndex = std::type_index;
	using QuickActionFactory = std::function<QuickActionBase* ()>;
	using SegmentFactory = std::function<SegmentBase* ()>;
	
	struct QuickActionRegistryItem {
		QuickActionFactory factory;
		std::string typeId;
		std::wstring typeLabel;
		SegmentFactory segmentFactory;

		QuickActionRegistryItem(
			QuickActionFactory factory,
			std::string typeId,
			std::wstring typeLabel,
			SegmentFactory segmentFactory = nullptr
		)
			: factory(std::move(factory))
			, typeId(std::move(typeId))
			, typeLabel(std::move(typeLabel))
			, segmentFactory(std::move(segmentFactory))
		{}
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
			auto& actions = Actions();
			auto it = std::find_if(
				actions.begin(),
				actions.end(),
				[&typeId](const auto& pair) {
					return pair.second.typeId == typeId;
				}
			);
			if (it != actions.end()) {
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
			actions.insert({ type, std::move(quickAction) });
		}
	};
}

// Macro for actions without segment factory (simple actions)
#define REGISTER_QUICK_ACTION(CLASSNAME) \
namespace { \
	struct CLASSNAME##_QuickActionReg { \
		CLASSNAME##_QuickActionReg() { \
			ModMenuModule::QuickActionRegistry::Register<ModMenuModule::CLASSNAME>( \
				ModMenuModule::QuickActionRegistryItem( \
					[]() { return new ModMenuModule::CLASSNAME(); }, \
					ModMenuModule::CLASSNAME::GetTypeId(), \
					ModMenuModule::CLASSNAME::GetTypeLabel() \
				) \
			); \
		} \
	}; \
	static CLASSNAME##_QuickActionReg s_##CLASSNAME##_QuickActionReg; \
}

// Macro for actions with segment factory (actions that need configuration UI)
#define REGISTER_QUICK_ACTION_WITH_SEGMENT(CLASSNAME) \
namespace { \
	struct CLASSNAME##_QuickActionReg { \
		CLASSNAME##_QuickActionReg() { \
			ModMenuModule::QuickActionRegistry::Register<ModMenuModule::CLASSNAME>( \
				ModMenuModule::QuickActionRegistryItem( \
					[]() { return new ModMenuModule::CLASSNAME(); }, \
					ModMenuModule::CLASSNAME::GetTypeId(), \
					ModMenuModule::CLASSNAME::GetTypeLabel(), \
					[]() { return ModMenuModule::CLASSNAME::CreateSegmentInstance(); } \
				) \
			); \
		} \
	}; \
	static CLASSNAME##_QuickActionReg s_##CLASSNAME##_QuickActionReg; \
}
