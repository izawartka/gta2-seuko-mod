#include "persistence-manager.h"

PersistenceModule::PersistenceManager* PersistenceModule::PersistenceManager::m_instance = nullptr;

PersistenceModule::PersistenceManager::PersistenceManager() {
	assert(!m_instance && "PersistenceManager instance already exists");
	m_instance = this;
}

PersistenceModule::PersistenceManager::~PersistenceManager() {
	m_instance = nullptr;
}

PersistenceModule::PersistenceManager* PersistenceModule::PersistenceManager::GetInstance() {
	assert(m_instance != nullptr && "PersistenceManager not initialized!");
	return m_instance;
}

void PersistenceModule::PersistenceManager::SaveToFile() {
	std::ofstream ofs(PERSISTENCE_FILE, std::ios::binary | std::ios::trunc);
	if (!ofs) {
		spdlog::error("Failed to open persistence file for saving: {}", PERSISTENCE_FILE);
		return;
	}

	size_t entriesCount = m_savedValues.size();
	ofs.write(reinterpret_cast<const char*>(&entriesCount), sizeof(entriesCount));
	for (const auto& pair : m_savedValues) {
		const auto& key = pair.first;
		const auto& value = pair.second;
		size_t keyLength = key.size();
		ofs.write(reinterpret_cast<const char*>(&keyLength), sizeof(keyLength));
		ofs.write(key.data(), keyLength);
		size_t dataLength = value->data.size();
		ofs.write(reinterpret_cast<const char*>(&dataLength), sizeof(dataLength));
		if (dataLength > 0)
			ofs.write(reinterpret_cast<const char*>(value->data.data()), dataLength);
	}
	if (!ofs.good()) {
		spdlog::error("Error occurred while saving persistence data");
	}
}

void PersistenceModule::PersistenceManager::LoadFromFile() {
	std::ifstream ifs(PERSISTENCE_FILE, std::ios::binary);
	if (!ifs) {
		spdlog::warn("Persistence file not found: {}", PERSISTENCE_FILE);
		return;
	}

	ifs.seekg(0, std::ios::end);
	size_t fileSize = static_cast<size_t>(ifs.tellg());
	ifs.seekg(0, std::ios::beg);

	if (fileSize < sizeof(size_t)) {
		spdlog::warn("Persistence file is too small to be valid: {} bytes", fileSize);
		return;
	}

	size_t entriesCount = 0;
	ifs.read(reinterpret_cast<char*>(&entriesCount), sizeof(entriesCount));

	bool entryOk = true;
	for (size_t i = 0; i < entriesCount; ++i) {
		entryOk = false;
		size_t keyLength = 0;
		ifs.read(reinterpret_cast<char*>(&keyLength), sizeof(keyLength));
		if (ifs.eof()) break;
		if (keyLength == 0) {
			spdlog::warn("Invalid key length in persistence file: {}", keyLength);
			break;
		}
		std::string key(keyLength, '\0');
		ifs.read(&key[0], keyLength);
		if (ifs.eof()) break;
		size_t dataLength = 0;
		ifs.read(reinterpret_cast<char*>(&dataLength), sizeof(dataLength));
		if (ifs.eof()) break;
		std::vector<uint8_t> data(dataLength);
		if (dataLength > 0) {
			ifs.read(reinterpret_cast<char*>(data.data()), dataLength);
			if (ifs.eof()) break;
		}
		m_savedValues[key] = std::make_unique<SavedValue>(key, dataLength, data.data());
		entryOk = true;
	}
	if (!ifs.good() && !ifs.eof() && !entryOk) {
		spdlog::error("Error occurred while loading persistence data");
	}
	else {
		spdlog::info("Loaded {} entries from persistence file", entriesCount);
	}
}
