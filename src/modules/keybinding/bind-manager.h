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

	private:
		friend class RootModule;
		BindManager();
		~BindManager();
		BindManager(const BindManager&) = delete;
		BindManager& operator=(const BindManager&) = delete;

		KeyPtr SetBindNoLookup(const std::string& name, const Key& newKey);
		void SaveToPersistence() const;
		void LoadFromPersistence();

		static BindManager* m_instance;

		std::unordered_map<std::string, std::shared_ptr<Key>> m_keyBinds;
	};
}
