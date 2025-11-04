#include "bind-manager.h"

KeyBindingModule::BindManager* KeyBindingModule::BindManager::m_instance = nullptr;

KeyBindingModule::BindManager::BindManager() {
	assert(!m_instance, "BindManager instance already exists");
	m_instance = this;
}

KeyBindingModule::BindManager::~BindManager() {
	m_instance = nullptr;
}

KeyBindingModule::BindManager* KeyBindingModule::BindManager::GetInstance() {
	assert(m_instance != nullptr, "BindManager not initialized!");
	return m_instance;
}

const KeyBindingModule::Key* KeyBindingModule::BindManager::GetBind(const std::string& name) const
{
	auto it = m_keyBinds.find(name);
	if (it != m_keyBinds.end()) {
		return it->second.get();
	}
	return nullptr;
}

const KeyBindingModule::Key* KeyBindingModule::BindManager::GetOrCreateBind(const std::string& name, KeyBindingModule::Key defaultKey)
{
	auto it = m_keyBinds.find(name);
	if (it != m_keyBinds.end()) {
		return it->second.get();
	}
	spdlog::debug("Creating default key bind: {}", name);
	return SetBindNoLookup(name, defaultKey);
}

const KeyBindingModule::Key* KeyBindingModule::BindManager::SetBind(const std::string& name, KeyBindingModule::Key newKey)
{
	spdlog::debug("Setting key bind: {}", name);
	auto it = m_keyBinds.find(name);
	if (it != m_keyBinds.end()) {
		*(it->second) = newKey;
		return it->second.get();
	}
	return SetBindNoLookup(name, newKey);
}

KeyBindingModule::Key* KeyBindingModule::BindManager::SetBindNoLookup(const std::string& name, Key newKey)
{
	auto keyPtr = std::make_unique<KeyBindingModule::Key>(newKey);
	KeyBindingModule::Key* retPtr = keyPtr.get();
	m_keyBinds[name] = std::move(keyPtr);
	return retPtr;
}

void KeyBindingModule::BindManager::SaveToFile() const
{
	PersistenceModule::PersistenceManager* persistenceManager = PersistenceModule::PersistenceManager::GetInstance();

	spdlog::debug("Saving KeyBindingModule binds to persistence");
	std::vector<uint8_t> buffer;
	buffer.reserve(256);

	// bind count
	size_t sizeSize = sizeof(size_t);
	size_t bindCount = m_keyBinds.size();
	buffer.insert(buffer.end(), reinterpret_cast<uint8_t*>(&bindCount), reinterpret_cast<uint8_t*>(&bindCount) + sizeSize);

	for (const auto& pair : m_keyBinds) {
		const std::string& name = pair.first;
		const KeyBindingModule::Key& key = *(pair.second);
		// name length + name
		size_t nameLength = name.size();
		buffer.insert(buffer.end(), reinterpret_cast<uint8_t*>(&nameLength), reinterpret_cast<uint8_t*>(&nameLength) + sizeSize);
		buffer.insert(buffer.end(), reinterpret_cast<const uint8_t*>(name.data()), reinterpret_cast<const uint8_t*>(name.data()) + nameLength);
		// key data
		buffer.insert(buffer.end(), reinterpret_cast<const uint8_t*>(&key), reinterpret_cast<const uint8_t*>(&key) + sizeof(KeyBindingModule::Key));
	}

	persistenceManager->SaveRaw("KeyBindingModule_BindManager_Binds", buffer.data(), buffer.size());
}

void KeyBindingModule::BindManager::LoadFromFile()
{
	spdlog::debug("Loading KeyBindingModule binds from persistence");
	uint8_t* data = nullptr;
	size_t size = 0;
	if (!PersistenceModule::PersistenceManager::GetInstance()->LoadRaw("KeyBindingModule_BindManager_Binds", data, size)) {
		spdlog::debug("No saved binds found in persistence");
		return;
	}

	size_t offset = 0;
	auto readSizeT = [&data, &size, &offset]() -> std::optional<size_t> {
		if (offset + sizeof(size_t) > size) {
			return std::nullopt;
		}
		size_t value;
		std::memcpy(&value, data + offset, sizeof(size_t));
		offset += sizeof(size_t);
		return value;
	};

	auto readKey = [&data, &size, &offset]() -> std::optional<KeyBindingModule::Key> {
		if (offset + sizeof(KeyBindingModule::Key) > size) {
			return std::nullopt;
		}
		KeyBindingModule::Key key;
		std::memcpy(&key, data + offset, sizeof(KeyBindingModule::Key));
		offset += sizeof(KeyBindingModule::Key);
		return key;
	};

	auto bindCountOpt = readSizeT();
	if (!bindCountOpt) {
		spdlog::error("Failed to read bind count from persistence data");
		delete[] data;
		return;
	}

	std::unordered_map<std::string, std::unique_ptr<KeyBindingModule::Key>> loadedBinds;
	bool readError = false;

	size_t bindCount = *bindCountOpt;
	for (size_t i = 0; i < bindCount; ++i) {
		auto nameLengthOpt = readSizeT();
		if (!nameLengthOpt) {
			readError = true;
			break;
		}
		size_t nameLength = *nameLengthOpt;
		if (offset + nameLength > size) {
			readError = true;
			break;
		}
		std::string name(reinterpret_cast<char*>(data + offset), nameLength);
		offset += nameLength;
		auto keyOpt = readKey();
		if (!keyOpt) {
			readError = true;
			break;
		}
		KeyBindingModule::Key key = *keyOpt;
		loadedBinds[name] = std::make_unique<KeyBindingModule::Key>(key);
	}

	if (offset != size) {
		readError = true;
	}

	delete[] data;
	if (readError) {
		spdlog::error("Error occurred while loading binds from persistence data");
		return;
	}

	m_keyBinds = std::move(loadedBinds);
}