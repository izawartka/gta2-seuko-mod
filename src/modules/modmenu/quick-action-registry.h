#pragma once
#include "quick-action-base.h"

namespace ModMenuModule {
	using QuickActionTypeIndex = std::type_index;
	using QuickActionFactory = std::function<QuickActionBase* ()>;
	using SegmentFactory = std::function<SegmentBase* ()>;
}

namespace {
	template<typename QuickActionT>
	ModMenuModule::QuickActionBase* QuickActionFactoryFunc() {
		return new QuickActionT();
	}

	template<typename QuickActionT>
	const std::string& GetQuickActionTypeId() {
		return QuickActionT::GetTypeId();
	}

	template<typename QuickActionT>
	const std::wstring& GetQuickActionTypeLabel() {
		return QuickActionT::GetTypeLabel();
	}

	template<typename QuickActionT, typename = void>
	struct HasQuickActionCreateSegmentInstance : std::false_type {};

	template<typename QuickActionT>
	struct HasQuickActionCreateSegmentInstance<QuickActionT, std::void_t<decltype(QuickActionT::CreateSegmentInstance())>> : std::true_type {};

	template<typename QuickActionT>
	ModMenuModule::SegmentBase* SegmentFactoryFunc() {
		if constexpr (HasQuickActionCreateSegmentInstance<QuickActionT>::value) {
			return QuickActionT::CreateSegmentInstance();
		}
		else {
			return nullptr;
		}
	}

	template<typename QuickActionT, typename = void>
	struct HasQuickActionIsDeprecated : std::false_type {};

	template<typename QuickActionT>
	struct HasQuickActionIsDeprecated<QuickActionT, std::void_t<decltype(QuickActionT::IsDeprecated())>> : std::true_type {};

	template<typename QuickActionT>
	constexpr ModMenuModule::SegmentFactory GetSegmentFactory() {
		if constexpr (HasQuickActionCreateSegmentInstance<QuickActionT>::value) {
			return SegmentFactoryFunc<QuickActionT>;
		}
		else {
			return nullptr;
		}
	}

	template<typename QuickActionT>
	constexpr bool IsQuickActionDeprecated() {

		if constexpr (HasQuickActionIsDeprecated<QuickActionT>::value) {
			return QuickActionT::IsDeprecated();
		}
		else {
			return false;
		}
	}
}

namespace ModMenuModule {
	class SegmentBase;

	class QuickActionRegistry {
	private:
		friend class QuickActionManager;

		struct RegistryItem {
			QuickActionFactory factory;
			std::string typeId;
			std::wstring typeLabel;
			SegmentFactory segmentFactory = nullptr;
			bool deprecated = false;

			RegistryItem() = default;

			RegistryItem(
				QuickActionFactory factory,
				std::string typeId,
				std::wstring typeLabel,
				SegmentFactory segmentFactory = nullptr,
				bool deprecated = false
			)
				: factory(std::move(factory))
				, typeId(std::move(typeId))
				, typeLabel(std::move(typeLabel))
				, segmentFactory(std::move(segmentFactory))
				, deprecated(deprecated)
			{
			}
		};

		static std::unordered_map<QuickActionTypeIndex, RegistryItem>& Actions() {
			static std::unordered_map<QuickActionTypeIndex, RegistryItem> actions;
			return actions;
		}

		static std::vector<QuickActionTypeIndex>& SortedIndiciesCache() {
			static std::vector<QuickActionTypeIndex> cache;
			return cache;
		}

		static std::vector<QuickActionTypeIndex>& SortedNonDeprecatedIndiciesCache() {
			static std::vector<QuickActionTypeIndex> cache;
			return cache;
		}

		static bool& IsSorted() {
			static bool sorted = false;
			return sorted;
		}

		static const std::vector<QuickActionTypeIndex>& GetAllTypeIndiciesSorted(bool excludeDeprecated) {
			if (!IsSorted()) {
				auto& actions = Actions();
				auto& cache = SortedIndiciesCache();
				auto& nonDeprecatedCache = SortedNonDeprecatedIndiciesCache();
				cache.clear();
				for (const auto& pair : actions) {
					cache.push_back(pair.first);
					if (!pair.second.deprecated) {
						nonDeprecatedCache.push_back(pair.first);
					}
				}
				std::sort(
					cache.begin(),
					cache.end(),
					[&actions](QuickActionTypeIndex a, QuickActionTypeIndex b) {
						return actions[a].typeLabel < actions[b].typeLabel;
					}
				);
				std::sort(
					nonDeprecatedCache.begin(),
					nonDeprecatedCache.end(),
					[&actions](QuickActionTypeIndex a, QuickActionTypeIndex b) {
						return actions[a].typeLabel < actions[b].typeLabel;
					}
				);
				IsSorted() = true;
			}

			if (excludeDeprecated) {
				return SortedNonDeprecatedIndiciesCache();
			}

			return SortedIndiciesCache();
		}

		static std::pair<QuickActionTypeIndex, const RegistryItem*> GetByTypeId(const std::string& typeId) {
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

		static const RegistryItem* GetByTypeIndex(QuickActionTypeIndex typeIndex) {
			auto& actions = Actions();
			auto it = actions.find(typeIndex);
			if (it == actions.end()) {
				return nullptr;
			}
			return &(it->second);
		}

	public:
		template<typename QuickActionT>
		static void Register() {
			static_assert(std::is_base_of<QuickActionBase, QuickActionT>::value, "QuickActionT must derive from QuickActionBase");

			auto& actions = Actions();
			QuickActionTypeIndex type = QuickActionTypeIndex(typeid(QuickActionT));
			if (actions.find(type) != actions.end()) return;

			RegistryItem item(
				QuickActionFactoryFunc<QuickActionT>,
				GetQuickActionTypeId<QuickActionT>(),
				GetQuickActionTypeLabel<QuickActionT>(),
				GetSegmentFactory<QuickActionT>(),
				IsQuickActionDeprecated<QuickActionT>()
			);

			actions.insert({ type, item });
		}
	};
}

#define REGISTER_QUICK_ACTION(CLASSNAME) \
namespace { \
	struct CLASSNAME##_QuickActionReg { \
		CLASSNAME##_QuickActionReg() { \
			ModMenuModule::QuickActionRegistry::Register<ModMenuModule::CLASSNAME>(); \
		} \
	}; \
	static CLASSNAME##_QuickActionReg s_##CLASSNAME##_QuickActionReg; \
}
