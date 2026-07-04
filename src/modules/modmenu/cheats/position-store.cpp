#include "position-store.h"
#include "../cheat-registry.h"
#include "../events/position-store-entries-update.h"

ModMenuModule::PositionStoreCheat* ModMenuModule::PositionStoreCheat::m_instance = nullptr;
static constexpr size_t POSITION_STORE_PERSISTENCE_VERSION = 1;

ModMenuModule::PositionStoreCheat::PositionStoreCheat() : ModMenuModule::CheatBase("")
{
	assert(m_instance == nullptr && "PositionStoreCheat instance already exists");
	m_instance = this;
}

ModMenuModule::PositionStoreCheat::~PositionStoreCheat()
{
	m_instance = nullptr;
}

ModMenuModule::PositionStoreCheat* ModMenuModule::PositionStoreCheat::GetInstance()
{
	assert(m_instance && "PositionStoreCheat not initialized!");
	return m_instance;
}

ModMenuModule::PositionStoreCheat::PositionId ModMenuModule::PositionStoreCheat::Create()
{
	PositionStoreEntry entry = {};
	return Create(entry);
}

ModMenuModule::PositionStoreCheat::PositionId ModMenuModule::PositionStoreCheat::Create(const PositionStoreEntry& entry)
{
	PositionId id = m_nextPositionId++;

	auto result = m_entries.emplace(id, entry);
	PositionStoreEntry& newEntry = result.first->second;
	ApplyNow(newEntry);
	UpdatePlayerUpdateSet(&newEntry, newEntry.updateFromPlayerPed);

	return id;
}

bool ModMenuModule::PositionStoreCheat::Update(PositionId id, const PositionStoreEntry& entry)
{
	auto it = m_entries.find(id);

	if (it == m_entries.end()) {
		spdlog::error("PositionStoreCheat: Position {} does not exist, cannot update", id);
		return false;
	}

	it->second = entry;
	PositionStoreEntry& newEntry = it->second;
	ApplyNow(newEntry);
	UpdatePlayerUpdateSet(&newEntry, newEntry.updateFromPlayerPed);

	return true;
}

bool ModMenuModule::PositionStoreCheat::Remove(PositionId id)
{
	auto it = m_entries.find(id);

	if (it == m_entries.end()) {
		spdlog::error("PositionStoreCheat: Position {} does not exist, cannot remove", id);
		return false;
	}

	UpdatePlayerUpdateSet(&it->second, false);
	m_entries.erase(it);
	return true;
}

const ModMenuModule::PositionStoreEntry* ModMenuModule::PositionStoreCheat::Get(PositionId id) const
{
	auto it = m_entries.find(id);

	if (it == m_entries.end()) {
		spdlog::error("PositionStoreCheat: Position {} does not exist, cannot get", id);
		return nullptr;
	}

	return &it->second;
}

void ModMenuModule::PositionStoreCheat::ApplyNow(PositionStoreEntry& entry)
{
	if (entry.updateFromPlayerPed) {
		ApplyFromPlayerNow(entry.value);
	}
	else if (entry.autoZ) {
		ApplyAutoZ(entry.value);
	}
}

void ModMenuModule::PositionStoreCheat::SaveToPersistence(const std::string& key, const PositionStoreEntry& entry)
{
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();
	size_t dataSize = 1 + sizeof(entry);
	std::unique_ptr<uint8_t[]> dataPtr = std::make_unique<uint8_t[]>(dataSize);

	dataPtr[0] = POSITION_STORE_PERSISTENCE_VERSION;
	memcpy(dataPtr.get() + 1, &entry, sizeof(entry));

	persistence->SaveRaw(key, dataPtr.get(), dataSize);
}

std::optional<ModMenuModule::PositionStoreEntry> ModMenuModule::PositionStoreCheat::LoadFromPersistence(const std::string& key)
{
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();
	std::unique_ptr<uint8_t[]> dataPtr = nullptr;
	size_t dataSize = 0;
	if (!persistence->LoadRaw(key, dataPtr, dataSize)) return std::nullopt;
	if (dataSize < 1) {
		spdlog::error("PositionStoreCheat::LoadFromPersistence: invalid data size");
		return std::nullopt;
	}
	uint8_t version = dataPtr[0];
	if (version != POSITION_STORE_PERSISTENCE_VERSION && !ConvertPersistence(dataPtr, dataSize, version)) {
		return std::nullopt;
	}
	if (dataSize != 1 + sizeof(PositionStoreEntry)) {
		spdlog::error("PositionStoreCheat::LoadFromPersistence: invalid data size for version {}", version);
		return std::nullopt;
	}
	PositionStoreEntry entry;
	memcpy(&entry, dataPtr.get() + 1, sizeof(PositionStoreEntry));
	return entry;
}

bool ModMenuModule::PositionStoreCheat::ConvertPersistence(std::unique_ptr<uint8_t[]>& dataPtr, size_t& dataSize, uint8_t version)
{
	spdlog::error("PositionStoreCheat::ConvertPersistence: unsupported version {}", version);
	return false;
}

void ModMenuModule::PositionStoreCheat::OnFirstEnable()
{

}

void ModMenuModule::PositionStoreCheat::OnEnable()
{
	AddEventListener<PlayerPosRotUpdateEvent>(&PositionStoreCheat::OnPlayerPosRotUpdate);
}

void ModMenuModule::PositionStoreCheat::OnDisable()
{
	RemoveEventListener<PlayerPosRotUpdateEvent>();
}

bool ModMenuModule::PositionStoreCheat::ApplyFromPlayerNow(PositionStoreValue& value)
{
	PlayerPosCheat* playerPosCheat = PlayerPosCheat::GetInstance();

	if (!playerPosCheat->IsEnabled()) {
		spdlog::warn("PositionStoreCheat: Cannot apply from player: PlayerPosCheat is not enabled");
		return false;
	}

	value.position = playerPosCheat->GetLastPosition().value_or(value.position);
	value.rotation = playerPosCheat->GetLastRotation().value_or(value.rotation);
	return true;
}

bool ModMenuModule::PositionStoreCheat::ApplyAutoZ(PositionStoreValue& value)
{
	Game::MapBlocks* mapBlocks = Game::Memory::GetMapBlocks();
	if (!mapBlocks) {
		spdlog::warn("PositionStoreCheat: Cannot apply Auto Z: MapBlocks is null");
		return false;
	}

	Game::SCR_f z = 0;
	Game::Functions::FindMaxZ(mapBlocks, 0, &z, value.position.x, value.position.y);

	if (z == 0) {
		spdlog::warn(
			"PositionStoreCheat: Cannot apply Auto Z: FindMaxZ returned Z=0 for position ({}, {})", 
			Game::Utils::ToFloat(value.position.x), 
			Game::Utils::ToFloat(value.position.y)
		);
		return false;
	}

	value.position.z = z;
	return true;
}

void ModMenuModule::PositionStoreCheat::UpdatePlayerUpdateSet(PositionStoreEntry* entry, bool add)
{
	if (add) {
		m_playerUpdateEntries.insert(entry);
	}
	else {
		m_playerUpdateEntries.erase(entry);
	}

	SetEnabled(!m_playerUpdateEntries.empty());
}

void ModMenuModule::PositionStoreCheat::OnPlayerPosRotUpdate(ModMenuModule::PlayerPosRotUpdateEvent& event)
{
	for (PositionStoreEntry* entry : m_playerUpdateEntries) {
		if (!entry->updateFromPlayerPed) continue;
		if (!event.GetPosition().has_value() || !event.GetRotation().has_value()) continue;
		entry->value.position = event.GetPosition().value();
		entry->value.rotation = event.GetRotation().value();
	}

	PositionStoreEntriesUpdateEvent entriesUpdateEvent;
	Core::EventManager::GetInstance()->Dispatch(entriesUpdateEvent);
}

REGISTER_CHEAT(PositionStoreCheat);
