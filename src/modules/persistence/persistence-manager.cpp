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

bool PersistenceModule::PersistenceManager::BackupFileIfExists(const std::wstring& filePath)
{
	if (GetFileAttributesW(filePath.c_str()) == INVALID_FILE_ATTRIBUTES) {
		return false;
	}

	std::wstring backupFilePath = filePath + PERSISTENCE_FILE_BACKUP_EXT;
	if (!CopyFileW(filePath.c_str(), backupFilePath.c_str(), FALSE)) {
		spdlog::error(
			L"Failed to create backup of persistence file from {} to {}. Error code: {}",
			filePath,
			backupFilePath,
			GetLastError()
		);
		return false;
	}

	spdlog::info(
		L"Created backup of persistence file at: {}",
		backupFilePath
	);
	return true;
}

std::wstring PersistenceModule::PersistenceManager::GetRootDirectory()
{
	wchar_t path[MAX_PATH];
	DWORD len = GetModuleFileNameW(nullptr, path, MAX_PATH);
	if (len == 0 || len == MAX_PATH) {
		spdlog::error("PersistenceManager: Failed to get host module file name");
		return L"";
	}

	std::wstring fullPath(path);

	size_t pos = fullPath.find_last_of(L"\\/");
	if (pos == std::wstring::npos) {
		spdlog::error("PersistenceManager: Invalid host module file path format");
		return L"";
	}

	return fullPath.substr(0, pos);
}

std::wstring PersistenceModule::PersistenceManager::GetLegacyRootDirectory()
{
	wchar_t buffer[MAX_PATH];
	DWORD len = GetCurrentDirectoryW(MAX_PATH, buffer);
	if (len == 0 || len == MAX_PATH) {
		spdlog::error("PersistenceManager: Failed to get legacy root directory");
		return L"";
	}

	return std::wstring(buffer);
}

void PersistenceModule::PersistenceManager::MoveLegacyFileIfExists()
{
	std::wstring legacyFilePath = GetLegacyRootDirectory() + L"\\" + PERSISTENCE_FILE;
	std::wstring newFilePath = GetRootDirectory() + L"\\" + PERSISTENCE_FILE;

	if (!BackupFileIfExists(legacyFilePath)) return;

	if (!MoveFileExW(legacyFilePath.c_str(), newFilePath.c_str(), MOVEFILE_COPY_ALLOWED | MOVEFILE_WRITE_THROUGH | MOVEFILE_REPLACE_EXISTING)) {
		spdlog::error(
			L"Failed to move legacy persistence file from {} to {}. Error code: {}",
			legacyFilePath,
			newFilePath,
			GetLastError()
		);
		return;
	}

	spdlog::info(
		L"Moved legacy persistence file from {} to {}",
		legacyFilePath,
		newFilePath
	);
}

void PersistenceModule::PersistenceManager::SaveToFile() {
	BackupFileIfExists(m_persistenceFilePath);

	std::ofstream ofs(m_persistenceFilePath, std::ios::binary | std::ios::trunc);
	if (!ofs) {
		spdlog::error(
			L"Failed to open persistence file for saving: {}", 
			m_persistenceFilePath
		);

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
	else {
		spdlog::info("Saved {} entries to persistence file", entriesCount);
	}
}

void PersistenceModule::PersistenceManager::LoadFromFile() {
	std::ifstream ifs(m_persistenceFilePath, std::ios::binary);
	if (!ifs) {
		spdlog::warn(L"Persistence file not found: {}", m_persistenceFilePath);
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

void PersistenceModule::PersistenceManager::UpdatePersistenceFilePath()
{
	std::wstring rootDir = GetRootDirectory();
	if (rootDir.empty()) {
		spdlog::error("Cannot update persistence file path due to invalid root directory");
		return;
	}

	m_persistenceFilePath = rootDir + L"\\" + PERSISTENCE_FILE;
	spdlog::debug(L"Persistence file path set to: {}", m_persistenceFilePath);
}
