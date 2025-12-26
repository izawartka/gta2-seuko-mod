#include "native-cheats-keeper.h"
#include "../cheat-registry.h"
#include "../../../events/game-tick.h"
#include "native-cheats-data.cpp"

ModMenuModule::NativeCheatsKeeperCheat* ModMenuModule::NativeCheatsKeeperCheat::m_instance = nullptr;

ModMenuModule::NativeCheatsKeeperCheat::NativeCheatsKeeperCheat() : ModMenuModule::CheatBase("Cheat_NativeCheatsKeeper_IsEnabled")
{
	assert(m_instance == nullptr && "NativeCheatsKeeperCheat instance already exists");
	m_instance = this;
}

ModMenuModule::NativeCheatsKeeperCheat::~NativeCheatsKeeperCheat()
{
	m_instance = nullptr;
}

ModMenuModule::NativeCheatsKeeperCheat* ModMenuModule::NativeCheatsKeeperCheat::GetInstance()
{
	assert(m_instance && "NativeCheatsKeeperCheat not initialized!");
	return m_instance;
}

const std::vector<ModMenuModule::NativeCheatCategory>& ModMenuModule::NativeCheatsKeeperCheat::GetAllNativeCheatCategories()
{
	return nativeCheatCategories;
}

const std::vector<ModMenuModule::NativeCheatCategoryDef>& ModMenuModule::NativeCheatsKeeperCheat::GetAllNativeCheatCategoryDefs()
{
	return nativeCheatCategoryDefs;
}

const std::vector<ModMenuModule::NativeCheatDef>& ModMenuModule::NativeCheatsKeeperCheat::GetAllNativeCheatDefs()
{
	return nativeCheatDefs;
}

const std::vector<const ModMenuModule::NativeCheatDef*>& ModMenuModule::NativeCheatsKeeperCheat::GetAllNativeCheatDefsByCategory(NativeCheatCategory category)
{
	return nativeCheatDefsByCategoryMap.at(category);
}

const std::vector<size_t>& ModMenuModule::NativeCheatsKeeperCheat::GetAllNativeCheatsByCategory(NativeCheatCategory category)
{
	return nativeCheatsByCategoryMap.at(category);
}

const ModMenuModule::NativeCheatCategoryDef* ModMenuModule::NativeCheatsKeeperCheat::GetNativeCheatCategoryDef(NativeCheatCategory category)
{
	auto it = nativeCheatCategoryDefMap.find(category);
	if (it != nativeCheatCategoryDefMap.end()) {
		return it->second;
	}
	return nullptr;
}

const ModMenuModule::NativeCheatDef* ModMenuModule::NativeCheatsKeeperCheat::GetNativeCheatDef(size_t address)
{
	auto it = nativeCheatDefMap.find(address);
	if (it != nativeCheatDefMap.end()) {
		return it->second;
	}
	return nullptr;
}

bool ModMenuModule::NativeCheatsKeeperCheat::SetCheat(const NativeCheatDef& cheat, NativeCheatState state)
{
	return SetCheat(cheat.address, state);
}

bool ModMenuModule::NativeCheatsKeeperCheat::SetCheat(size_t cheatAddress, NativeCheatState state)
{
	if (!IsEnabled()) {
		spdlog::warn("NativeCheatsKeeperCheat::SetCheat: Cheat keeper is not enabled, cannot set cheat state");
		return false;
	}
	if (cheatAddress >= sizeof(Game::Cheats)) {
		spdlog::error("NativeCheatsKeeperCheat::SetCheat: Invalid cheat address: {}", cheatAddress);
		return false;
	}
	spdlog::debug("Setting cheat {} state to {}", static_cast<size_t>(cheatAddress), static_cast<int>(state));
	SetCheatStateInternal(cheatAddress, state);
	return true;
}

ModMenuModule::NativeCheatState ModMenuModule::NativeCheatsKeeperCheat::GetCheatState(const NativeCheatDef& cheat) const
{
	if (!IsEnabled()) return NativeCheatState::Unchanged;
	if (cheat.address >= sizeof(Game::Cheats)) {
		spdlog::error("NativeCheatsKeeperCheat::GetCheatState: Invalid cheat address: {}", cheat.address);
		return NativeCheatState::Unchanged;
	}

	return m_cheatEntries[cheat.address].state;
}

bool ModMenuModule::NativeCheatsKeeperCheat::IsCheatEnabled(const NativeCheatDef& cheat) const
{
	if (cheat.address >= sizeof(Game::Cheats)) {
		spdlog::error("NativeCheatsKeeperCheat::IsCheatEnabled: Invalid cheat address: {}", cheat.address);
		return false;
	}

	Game::Cheats* cheats = Game::Memory::GetCheats();
	bool* cheatBytes = reinterpret_cast<bool*>(cheats);
	return cheatBytes[cheat.address];
}

void ModMenuModule::NativeCheatsKeeperCheat::ResetAll()
{
	spdlog::debug("NativeCheatsKeeperCheat::ResetAll: Resetting all cheats to Unchanged");
	for (size_t i = 0; i < sizeof(Game::Cheats); i++) {
		if (m_cheatEntries[i].state == NativeCheatState::Unchanged) continue;
		SetCheatStateInternal(i, NativeCheatState::Unchanged);
	}
}

void ModMenuModule::NativeCheatsKeeperCheat::OnAfterDebugFlags(AfterDebugFlagsEvent& event)
{
	for (size_t i = 0; i < sizeof(Game::Cheats); i++) {
		NativeCheatsKeeperEntry& entry = m_cheatEntries[i];
		if (entry.watched) {
			entry.watched->Update();
		}
	}
}

void ModMenuModule::NativeCheatsKeeperCheat::OnCheatValueChange(size_t cheatAddress, const std::optional<bool>& oldValue, const std::optional<bool>& newValue)
{
	NativeCheatsKeeperEntry& entry = m_cheatEntries[cheatAddress];
	if (entry.ignoreValueChange) {
		entry.ignoreValueChange = false;
		return;
	}

	spdlog::debug("NativeCheatsKeeperCheat::OnCheatValueChange: Cheat at address {} changed from {} to {}", cheatAddress,
		oldValue.has_value() ? (oldValue.value() ? "true" : "false") : "null",
		newValue.has_value() ? (newValue.value() ? "true" : "false") : "null"
	);

	if (!newValue.has_value()) return;
	entry.originalValue = newValue.value();
	entry.watched->SetValueNow(entry.state == NativeCheatState::ForceEnabled);
}

void ModMenuModule::NativeCheatsKeeperCheat::OnFirstEnable()
{
	LoadFromPersistence();
}

void ModMenuModule::NativeCheatsKeeperCheat::OnEnable()
{
	AddEventListener<AfterDebugFlagsEvent>(&NativeCheatsKeeperCheat::OnAfterDebugFlags);
}

void ModMenuModule::NativeCheatsKeeperCheat::OnDisable()
{
	RemoveEventListener<AfterDebugFlagsEvent>();
	SaveToPersistence();
	ResetAll();
}

void ModMenuModule::NativeCheatsKeeperCheat::SaveToPersistence() const
{
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();
	NativeCheatsPersistenceData persistenceData;
	for (size_t i = 0; i < sizeof(Game::Cheats); i++) {
		persistenceData.cheatStates[i] = m_cheatEntries[i].state;
	}
	persistence->Save<NativeCheatsPersistenceData>("Cheat_NativeCheatsKeeper_State", persistenceData);
}

void ModMenuModule::NativeCheatsKeeperCheat::LoadFromPersistence()
{
	PersistenceModule::PersistenceManager* persistence = PersistenceModule::PersistenceManager::GetInstance();
	auto persistenceData = persistence->LoadOptional<NativeCheatsPersistenceData>("Cheat_NativeCheatsKeeper_State");
	if (!persistenceData.has_value()) return;
	for (size_t i = 0; i < sizeof(Game::Cheats); i++) {
		SetCheatStateInternal(i, persistenceData->cheatStates[i]);
	}
}

void ModMenuModule::NativeCheatsKeeperCheat::SetCheatStateInternal(size_t cheatAddress, NativeCheatState state)
{
	Core::WatchManager* watchManager = Core::WatchManager::GetInstance();
	bool& cheatValue = (reinterpret_cast<bool*>(Game::Memory::GetCheats()))[cheatAddress];
	NativeCheatsKeeperEntry& entry = m_cheatEntries[cheatAddress];

	entry.state = state;
	if (state == NativeCheatState::Unchanged) {
		if (entry.watched) {
			watchManager->Unwatch(entry.watched);
			entry.watched = nullptr;
			cheatValue = entry.originalValue;
		}

		return;
	}

	if (entry.watched == nullptr) {
		entry.originalValue = cheatValue;
		entry.ignoreValueChange = true;
		entry.watched = watchManager->Watch<PreGameTickEvent, bool, bool*>(
			[cheatAddress]() { return &(reinterpret_cast<bool*>(Game::Memory::GetCheats()))[cheatAddress]; },
			std::bind(&ModMenuModule::NativeCheatsKeeperCheat::OnCheatValueChange, this, cheatAddress, std::placeholders::_1, std::placeholders::_2)
		);
		entry.watched->Update();
	}

	entry.watched->SetValueNow(state == NativeCheatState::ForceEnabled);
}

REGISTER_CHEAT(NativeCheatsKeeperCheat)
