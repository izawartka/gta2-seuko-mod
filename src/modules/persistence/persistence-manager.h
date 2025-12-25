#pragma once
#include "common.h"

constexpr const char* PERSISTENCE_FILE = "seukomod.dat";

namespace PersistenceModule {
	class PersistenceManager {
	public:
		static PersistenceManager* GetInstance();

		// Suggested key format: "ModuleName_ClassName_VariableName"
		template <typename T>
		void Save(const std::string& key, const T& value) {
			spdlog::debug("Saving value with key: {}", key);
			SaveInternal<T>(key, value);
		}

		void SaveRaw(const std::string& key, const uint8_t* data, size_t size) {
			spdlog::debug("Saving raw data with key: {}", key);
			m_savedValues[key] = std::make_unique<SavedValue>(key, size, data);
		}

		template <typename T>
		T Load(const std::string& key, const T& defaultValue) {
			spdlog::debug("Loading value with key: {}", key);
			auto result = LoadInternal<T>(key);
			if (result.has_value()) {
				return result.value();
			} else {
				spdlog::debug("Returning default value");
				return defaultValue;
			}
		}

		template <typename T>
		std::optional<T> LoadOptional(const std::string& key) {
			spdlog::debug("Loading value with key: {}", key);
			return LoadInternal<T>(key);
		}

		bool LoadRaw(const std::string& key, std::unique_ptr<uint8_t[]>& data, size_t& size) {
			spdlog::debug("Loading raw data with key: {}", key);
			auto it = m_savedValues.find(key);
			if (it == m_savedValues.end()) {
				spdlog::warn("Key not found");
				return false;
			}
			auto savedValue = it->second.get();
			size = savedValue->size;
			data = std::make_unique<uint8_t[]>(size);
			std::memcpy(data.get(), savedValue->data.data(), size);
			return true;
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

		template <typename T>
		void SaveInternal(const std::string& key, const T& value) {
			std::vector<uint8_t> data(sizeof(T));
			std::memcpy(data.data(), &value, sizeof(T));
			m_savedValues[key] = std::make_unique<SavedValue>(key, sizeof(T), data.data());
		}

		template <>
		void SaveInternal<std::wstring>(const std::string& key, const std::wstring& value) {
			size_t dataSize = value.size() * sizeof(wchar_t);
			std::vector<uint8_t> data(dataSize);
			std::memcpy(data.data(), value.data(), dataSize);
			m_savedValues[key] = std::make_unique<SavedValue>(key, dataSize, data.data());
		}

		template <>
		void SaveInternal<std::string>(const std::string& key, const std::string& value) {
			size_t dataSize = value.size() * sizeof(char);
			std::vector<uint8_t> data(dataSize);
			std::memcpy(data.data(), value.data(), dataSize);
			m_savedValues[key] = std::make_unique<SavedValue>(key, dataSize, data.data());
		}

		template <typename T>
		std::optional<T> LoadInternal(const std::string& key) {
			auto it = m_savedValues.find(key);
			if (it == m_savedValues.end()) {
				spdlog::warn("Key not found");
				return std::nullopt;
			}
			auto savedValue = it->second.get();
			if (savedValue->data.size() != sizeof(T)) {
				spdlog::error("Size mismatch: {} vs {}", savedValue->data.size(), sizeof(T));
				return std::nullopt;
			}
			T value;
			std::memcpy(&value, savedValue->data.data(), sizeof(T));
			return value;
		}

		template <>
		std::optional<std::wstring> LoadInternal<std::wstring>(const std::string& key) {
			auto it = m_savedValues.find(key);
			if (it == m_savedValues.end()) {
				spdlog::warn("Key not found");
				return std::nullopt;
			}
			auto savedValue = it->second.get();
			size_t wcharCount = savedValue->data.size() / sizeof(wchar_t);
			std::wstring value(wcharCount, L'\0');
			std::memcpy(&value[0], savedValue->data.data(), savedValue->data.size());
			return value;
		}

		template <>
		std::optional<std::string> LoadInternal<std::string>(const std::string& key) {
			auto it = m_savedValues.find(key);
			if (it == m_savedValues.end()) {
				spdlog::warn("Key not found");
				return std::nullopt;
			}
			auto savedValue = it->second.get();
			size_t charCount = savedValue->data.size() / sizeof(char);
			std::string value(charCount, '\0');
			std::memcpy(&value[0], savedValue->data.data(), savedValue->data.size());
			return value;
		}

		std::unordered_map<std::string, std::unique_ptr<SavedValue>> m_savedValues;
	};
}
