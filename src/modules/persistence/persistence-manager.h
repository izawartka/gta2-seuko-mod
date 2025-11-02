#pragma once
#include "common.h"

constexpr const char* PERSISTENCE_FILE = "seukomod.dat";

namespace PersistenceModule {
	class PersistenceManager {
	public:
		static PersistenceManager* GetInstance();

		// Suggested key format: "ModuleName_ClassName_VariableName"
		template <typename T>
		void Save(std::string key, T value) {
			spdlog::debug("Saving value with key: {}", key);
			std::vector<uint8_t> data(sizeof(T));
			std::memcpy(data.data(), &value, sizeof(T));
			m_savedValues[key] = std::make_unique<SavedValue>(key, sizeof(T), data.data());
		}

		template <typename T>
		T Load(std::string key, T defaultValue) {
			spdlog::debug("Loading value with key: {}", key);
			auto it = m_savedValues.find(key);
			if (it == m_savedValues.end()) {
				spdlog::debug("Key not found, returning default value");
				return defaultValue;
			}

			auto savedValue = it->second.get();

			if (savedValue->data.size() != sizeof(T)) {
				spdlog::warn("Size mismatch: {} vs {}, returning default value", savedValue->data.size(), sizeof(T));
				return defaultValue;
			}

			T value;
			std::memcpy(&value, savedValue->data.data(), sizeof(T));
			return value;
		}

		void SaveToFile();
		void LoadFromFile();

	private:
		friend class RootModule;
		PersistenceManager();
		~PersistenceManager();
		PersistenceManager(const PersistenceManager&) = delete;
		PersistenceManager& operator=(const PersistenceManager&) = delete;

		static PersistenceManager* m_instance;

		struct SavedValue {
			SavedValue(std::string k, size_t s, const uint8_t* d) : key(k), size(s), data(d, d + s) {}
			virtual ~SavedValue() = default;

			std::string key;
			size_t size;
			std::vector<uint8_t> data;
		};

		std::unordered_map<std::string, std::unique_ptr<SavedValue>> m_savedValues;
	};
}
