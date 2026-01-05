#include "saved-cars.h"
#include "../utils/spawn-car-at-player.h"
#include "../../../game/game.h"
#include "../cheat-registry.h"

ModMenuModule::SavedCarsCheat* ModMenuModule::SavedCarsCheat::m_instance = nullptr;

ModMenuModule::SavedCarsCheat::SavedCarsCheat() : ModMenuModule::CheatBase("")
{
	assert(m_instance == nullptr && "SavedCarsCheat instance already exists");
	m_instance = this;
}

ModMenuModule::SavedCarsCheat::~SavedCarsCheat()
{
	m_instance = nullptr;
}

ModMenuModule::SavedCarsCheat* ModMenuModule::SavedCarsCheat::GetInstance()
{
	assert(m_instance && "SavedCarsCheat not initialized!");
	return m_instance;
}

bool ModMenuModule::SavedCarsCheat::SaveCar(std::wstring name, Game::Car* car)
{
	if (!car) {
		spdlog::error("SavedCarsCheat::SaveCar: Car pointer is null");
		return false;
	}

	if (name.empty()) {
		spdlog::warn("SavedCarsCheat::SaveCar: Name cannot be empty");
		return false;
	}

	if (m_entries.find(name) != m_entries.end()) {
		spdlog::warn("SavedCarsCheat::SaveCar: Car with that name already exists");
		return false;
	}

	SavedCarsCheatEntry entry = CreateEntry(car);
	m_entries[name] = entry;

	spdlog::info("SavedCarsCheat::SaveCar: Car saved");
	return true;
}

std::optional<ModMenuModule::SavedCarsCheatEntry> ModMenuModule::SavedCarsCheat::GetCar(std::wstring name)
{
	auto it = m_entries.find(name);
	if (it == m_entries.end()) {
		return std::nullopt;
	}
	return it->second;
}

bool ModMenuModule::SavedCarsCheat::SpawnCar(std::wstring name)
{
	auto entryOpt = GetCar(name);
	if (!entryOpt.has_value()) {
		spdlog::warn("SavedCarsCheat::SpawnCar: Car not found");
		return false;
	}

	return SpawnEntry(entryOpt.value());
}

bool ModMenuModule::SavedCarsCheat::DeleteCar(std::wstring name)
{
	auto it = m_entries.find(name);
	if (it == m_entries.end()) {
		spdlog::warn("SavedCarsCheat::DeleteCar: Car not found");
		return false;
	}
	m_entries.erase(it);
	spdlog::info("SavedCarsCheat::DeleteCar: Car deleted");
	return true;
}

std::vector<std::wstring> ModMenuModule::SavedCarsCheat::GetSavedCarsList()
{
	std::vector<std::wstring> list;
	list.reserve(m_entries.size());
	for (const auto& pair : m_entries) {
		list.push_back(pair.first);
	}
	return list;
}

void ModMenuModule::SavedCarsCheat::OnFirstEnable()
{
	LoadFromPersistence();
}

void ModMenuModule::SavedCarsCheat::OnEnable()
{
}

void ModMenuModule::SavedCarsCheat::OnDisable()
{
	SaveToPersistence();
}

ModMenuModule::SavedCarsCheatEntry ModMenuModule::SavedCarsCheat::CreateEntry(Game::Car* car)
{
	SavedCarsCheatEntry entry;
	entry.model = car->carModel;
	entry.remap = car->sprite ? car->sprite->carColor : 0;
	entry.palette = car->sprite ? car->sprite->paletteBase : Game::PALETTE_BASE::PALETTE_BASE_SPRITE;
	entry.deltasBitstate = car->deltasBitstate;
	entry.carDamage = car->carDamage;
	entry.physicsBitmask = car->physicsBitmask;

	entry.hasTankCannon = Game::Utils::GetCarRoofWithSprite(car->roof, 546) != nullptr;
	entry.hasWaterCannon = Game::Utils::GetCarRoofWithSprite(car->roof, 278) != nullptr;
	entry.hasJeepTurret = Game::Utils::GetCarRoofWithSprite(car->roof, 285) != nullptr;

	for (int i = 0; i < 13; i++) {
		Game::CAR_WEAPON_INDEX weaponType = static_cast<Game::CAR_WEAPON_INDEX>(15 + i);
		Game::WEAPON_STRUCT* weaponStruct = Game::Functions::GetCarWeaponStruct(car, weaponType);
		if (weaponStruct) {
			entry.carWeaponsAmmo[i] = weaponStruct->ammo;
		}
		else {
			entry.carWeaponsAmmo[i] = 0;
		}
	}

	return entry;
}

bool ModMenuModule::SavedCarsCheat::SpawnEntry(const SavedCarsCheatEntry& entry)
{
	Game::Car* car = ModMenuModule::Utils::SpawnCarAtPlayer(entry.model, entry.remap, entry.palette);

	if (!car) {
		return false;
	}

	car->deltasBitstate = entry.deltasBitstate;
	car->carDamage = entry.carDamage;
	car->physicsBitmask = entry.physicsBitmask;

	if (entry.hasTankCannon) {
		Game::Utils::AddCarRoofForWeapon(car, Game::WEAPON_VEHICLE_TANK_CANNON);
	}
	if (entry.hasWaterCannon) {
		Game::Utils::AddCarRoofForWeapon(car, Game::WEAPON_VEHICLE_WATER_CANNON);
	}
	if (entry.hasJeepTurret) {
		Game::Utils::AddCarRoofForWeapon(car, Game::WEAPON_VEHICLE_JEEP_TURRET);
	}

	for (int i = 0; i < 13; i++) {
		short ammo = entry.carWeaponsAmmo[i];
		Game::CAR_WEAPON_INDEX weaponType = static_cast<Game::CAR_WEAPON_INDEX>(15 + i);
		if(ammo != 0) Game::Functions::CarAddWeapon(weaponType, 1, car); // a bit hacky way to safely create weapon struct
		Game::WEAPON_STRUCT* weaponStruct = Game::Functions::GetCarWeaponStruct(car, weaponType);

		if(weaponStruct) {
			weaponStruct->ammo = ammo;
		}
	}

	return true;
}

void ModMenuModule::SavedCarsCheat::SaveToPersistence() const
{
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();

	size_t totalSize = sizeof(size_t) * 2;
	for (const auto& pair : m_entries) {
		totalSize += sizeof(size_t);
		totalSize += pair.first.size() * sizeof(wchar_t);
		totalSize += sizeof(SavedCarsCheatEntry);
	}

	std::unique_ptr<uint8_t[]> dataPtr = std::make_unique<uint8_t[]>(totalSize);
	size_t offset = 0;

	auto write = [&dataPtr, &offset](const void* src, size_t size) {
		std::memcpy(dataPtr.get() + offset, src, size);
		offset += size;
	};

	size_t version = 1;
	write(&version, sizeof(size_t));

	size_t entryCount = m_entries.size();
	write(&entryCount, sizeof(size_t));

	for (const auto& pair : m_entries) {
		size_t nameLength = pair.first.size();
		write(&nameLength, sizeof(size_t));
		write(pair.first.data(), nameLength * sizeof(wchar_t));
		write(&pair.second, sizeof(SavedCarsCheatEntry));
	}

	persistence->SaveRaw("Cheat_SavedCars_Entries", dataPtr.get(), totalSize);
	spdlog::debug("SavedCarsCheat: Saved {} car entries to persistence", entryCount);
}

void ModMenuModule::SavedCarsCheat::LoadFromPersistence()
{
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();

	std::unique_ptr<uint8_t[]> dataPtr = nullptr;
	size_t size = 0;
	if (!persistence->LoadRaw("Cheat_SavedCars_Entries", dataPtr, size)) {
		spdlog::debug("SavedCarsCheat: No saved cars found in persistence");
		return;
	}

	uint8_t* data = dataPtr.get();
	size_t offset = 0;

	auto readSizeT = [&data, &size, &offset]() -> std::optional<size_t> {
		if (offset + sizeof(size_t) > size) return std::nullopt;
		size_t value;
		std::memcpy(&value, data + offset, sizeof(size_t));
		offset += sizeof(size_t);
		return value;
	};

	auto versionOpt = readSizeT();
	if (!versionOpt) {
		spdlog::error("SavedCarsCheat: Failed to read version from persistence");
		return;
	}

	if (*versionOpt != 1) {
		spdlog::error("SavedCarsCheat: Unsupported version {} in persistence", *versionOpt);
		return;
	}

	auto entryCountOpt = readSizeT();
	if (!entryCountOpt) {
		spdlog::error("SavedCarsCheat: Failed to read entry count from persistence");
		return;
	}

	size_t entryCount = *entryCountOpt;
	std::unordered_map<std::wstring, SavedCarsCheatEntry> loadedEntries;
	bool readError = false;

	for (size_t i = 0; i < entryCount; ++i) {
		auto nameLengthOpt = readSizeT();
		if (!nameLengthOpt) {
			readError = true;
			break;
		}

		size_t nameLength = *nameLengthOpt;
		size_t nameBytes = nameLength * sizeof(wchar_t);
		if (offset + nameBytes > size) {
			readError = true;
			break;
		}

		std::wstring name(reinterpret_cast<wchar_t*>(data + offset), nameLength);
		offset += nameBytes;

		if (offset + sizeof(SavedCarsCheatEntry) > size) {
			readError = true;
			break;
		}

		SavedCarsCheatEntry entry;
		std::memcpy(&entry, data + offset, sizeof(SavedCarsCheatEntry));
		offset += sizeof(SavedCarsCheatEntry);

		loadedEntries[name] = entry;
	}

	if (offset != size) {
		readError = true;
	}

	if (readError) {
		spdlog::error("SavedCarsCheat: Error occurred while loading entries from persistence");
		return;
	}

	m_entries = std::move(loadedEntries);
	spdlog::info("SavedCarsCheat: Loaded {} car entries from persistence", entryCount);
}

REGISTER_CHEAT(SavedCarsCheat)
