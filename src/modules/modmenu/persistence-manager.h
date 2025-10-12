#pragma once
#include "common.h"

namespace ModMenuModule {
	class PersistenceManager {
	public:
		static PersistenceManager* GetInstance();

		template <typename T>
		void Save(std::string key, T value) {
			spdlog::debug("Saving value with key: {}", key);
			m_savedValues[key] = std::make_unique<SavedValue<T>>(key, value);
		}

		template <typename T>
		T Load(std::string key, T defaultValue) {
			spdlog::debug("Loading value with key: {}", key);
			auto it = m_savedValues.find(key);
			if (it == m_savedValues.end()) {
				spdlog::debug("Key not found, returning default value");
				return defaultValue;
			}
			auto savedValueBase = it->second.get();
			if (savedValueBase->type != typeid(T)) {
				spdlog::warn("Type mismatch for key: {}, returning default value", key);
				return defaultValue;
			}
			auto savedValue = static_cast<SavedValue<T>*>(savedValueBase);
			return savedValue->value;
		}
	private:
		friend class RootModule;
		PersistenceManager();
		~PersistenceManager();
		PersistenceManager(const PersistenceManager&) = delete;
		PersistenceManager& operator=(const PersistenceManager&) = delete;

		static PersistenceManager* m_instance;

		struct SavedValueBase {
			SavedValueBase() = default;
			virtual ~SavedValueBase() = default;

			std::string key;
			std::type_index type = typeid(void);
		};

		template <typename T>
		struct SavedValue : SavedValueBase {
			SavedValue(std::string k, T v) : value(v) {
				key = k;
				type = typeid(T);
			}

			T value;
		};

		std::unordered_map<std::string, std::unique_ptr<SavedValueBase>> m_savedValues;
	};
}
