#pragma once
#include "common.h"
#include "quick-action-base.h"
#include "quick-action-registry.h"

namespace ModMenuModule {
	using QuickActionId = size_t;
	
	struct QuickActionInfo {
		KeyBindingModule::Key key = {};
		const std::wstring& defaultLabel; // ignored in Update
		std::wstring customLabel;
		QuickActionTypeIndex typeIndex; // ignored in Update

		const std::wstring& GetLabel() const {
			return customLabel.empty() ? defaultLabel : customLabel;
		}

		QuickActionInfo(KeyBindingModule::Key key, const std::wstring& defaultLabel, const std::wstring& customLabel, QuickActionTypeIndex typeIndex)
			: key(key), defaultLabel(defaultLabel), customLabel(customLabel), typeIndex(typeIndex) {
		}

		QuickActionInfo& operator=(const QuickActionInfo& other) {
			if (this != &other) {
				key = other.key;
				const_cast<std::wstring&>(defaultLabel) = other.defaultLabel;
				customLabel = other.customLabel;
				typeIndex = other.typeIndex;
			}
			return *this;
		}
	};

	class QuickActionManager : public Core::EventListenerSupport {
	public:
		static QuickActionManager* GetInstance();
		static std::vector<QuickActionTypeIndex> GetAllTypes();
		static const std::wstring& GetTypeLabel(QuickActionTypeIndex typeIndex);
		
		static bool HasSegmentFactory(QuickActionTypeIndex typeIndex);
		static SegmentBase* CreateSegment(QuickActionTypeIndex typeIndex);

		std::optional<QuickActionInfo> GetInfo(QuickActionId actionId) const;
		std::vector<QuickActionId> GetAll() const;

		bool SetDataFromSegmentData(QuickActionId actionId, SegmentBase* segment);
		bool SetSegmentDataFromData(QuickActionId actionId, SegmentBase* segment);
		QuickActionId Add(KeyBindingModule::Key key, QuickActionTypeIndex typeIndex);
		void Update(QuickActionId actionId, const QuickActionInfo& info);
		void Remove(QuickActionId actionId);

	private:
		friend class RootModule;
		QuickActionManager();
		virtual ~QuickActionManager();
		QuickActionManager(const QuickActionManager&) = delete;
		QuickActionManager& operator=(const QuickActionManager&) = delete;

		static std::string GetKeyBindName(QuickActionId actionId);

		void Attach();
		void Detach();

		struct QuickActionEntry {
			const KeyBindingModule::Key* keyBind = nullptr;
			std::wstring customLabel = L"";
			std::type_index typeIndex = typeid(void);
			std::unique_ptr<QuickActionBase> action = nullptr;
		};

		struct AddQuickActionData {
			QuickActionId actionId = 0;
			KeyBindingModule::Key key; // not used by persistence
			QuickActionTypeIndex typeIndex = std::type_index(typeid(void)); 
			const QuickActionRegistryItem* actionItem = nullptr; 
			std::wstring customLabel = L"";
			std::vector<uint8_t> serializedData = {}; // only used for persistence
		};

		void OnKeyDown(KeyDownEvent& event);

		QuickActionEntry* GetQuickActionEntry(QuickActionId actionId);
		void AddQuickActionInternal(const AddQuickActionData& data);
		void SaveToPersistence() const;
		void LoadFromPersistence();

		static QuickActionManager* m_instance;

		QuickActionId m_nextQuickActionId = 1;
		std::unordered_map<QuickActionId, QuickActionEntry> m_quickActions = {};
	};
}
