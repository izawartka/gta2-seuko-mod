#pragma once
#include "common.h"
#include "key.h"

namespace KeyBindingModule {
	using KeyPtr = std::weak_ptr<const Key>;

	class BindManager {
	public:
		static BindManager* GetInstance();

		KeyPtr GetBind(const std::string& name) const;
		KeyPtr GetOrCreateBind(const std::string& name, const Key& defaultKey);
		KeyPtr SetBind(const std::string& name, const Key& newKey);
		bool RemoveBind(const std::string& name);
		std::vector<std::string> GetBindsByKey(const Key& key) const;
		bool CheckKeyIsInUse(const Key& key, const std::optional<std::string>& ignoreBindName = std::nullopt) const;

	private:
		friend class RootModule;
		BindManager();
		~BindManager();
		BindManager(const BindManager&) = delete;
		BindManager& operator=(const BindManager&) = delete;

		KeyPtr SetBindNoLookup(const std::string& name, const Key& newKey);
		void DispatchKeyBindUpdateEvent(const std::string& name);
		void SaveToPersistence() const;
		void LoadFromPersistence();

		static BindManager* m_instance;

		std::unordered_map<std::string, std::shared_ptr<Key>> m_keyBinds;
	};
}
