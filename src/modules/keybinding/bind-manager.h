#pragma once
#include "common.h"
#include "key.h"

namespace KeyBindingModule {
	class BindManager {
	public:
		static BindManager* GetInstance();

		const Key* GetBind(const std::string& name) const;
		const Key* GetOrCreateBind(const std::string& name, Key defaultKey);
		const Key* SetBind(const std::string& name, Key newKey);

	private:
		friend class RootModule;
		BindManager();
		~BindManager();
		BindManager(const BindManager&) = delete;
		BindManager& operator=(const BindManager&) = delete;

		Key* SetBindNoLookup(const std::string& name, Key newKey);
		void SaveToFile() const;
		void LoadFromFile();

		static BindManager* m_instance;

		std::unordered_map<std::string, std::unique_ptr<Key>> m_keyBinds;
	};
}
