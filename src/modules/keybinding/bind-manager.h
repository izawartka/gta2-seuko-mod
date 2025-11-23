#pragma once
#include "common.h"
#include "key.h"

namespace KeyBindingModule {
	class BindManager {
	public:
		static BindManager* GetInstance();

		const Key* GetBind(const std::string& name) const;
		const Key* GetOrCreateBind(const std::string& name, const Key& defaultKey);
		const Key* SetBind(const std::string& name, const Key& newKey);
		bool RemoveBind(const std::string& name);

	private:
		friend class RootModule;
		BindManager();
		~BindManager();
		BindManager(const BindManager&) = delete;
		BindManager& operator=(const BindManager&) = delete;

		Key* SetBindNoLookup(const std::string& name, const Key& newKey);
		void SaveToPersistence() const;
		void LoadFromPersistence();

		static BindManager* m_instance;

		std::unordered_map<std::string, std::unique_ptr<Key>> m_keyBinds;
	};
}
